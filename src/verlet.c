#include "verlet.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define GRAVITY -15.0f
#define THREAD_COUNT 8

void applyForces(VerletObject* objects, int size)
{
    for (int i = 0; i < size; i++) {
        objects[i].acceleration[1] += GRAVITY;
    }
}

void handleCollision(VerletObject* a, VerletObject* b)
{
    mfloat_t axis[VEC3_SIZE];
    vec3_subtract(axis, a->current, b->current);
    mfloat_t dist = vec3_length(axis);
    if (dist < a->radius + b->radius) {
        mfloat_t norm[VEC3_SIZE];
        vec3_divide_f(norm, axis, dist);
        mfloat_t delta = a->radius + b->radius - dist;
        vec3_multiply_f(norm, norm, 0.5 * delta);
        vec3_add(a->current, a->current, norm);
        vec3_subtract(b->current, b->current, norm);
    }
}

void applyCollisions(VerletObject* objects, int size)
{
    for (int a = 0; a < size; a++) {
        for (int b = 0; b < size; b++) {
            if (a != b) {
                handleCollision(&objects[a], &objects[b]);
            }
        }
    }
}

#define DIMENSION 58 // CONTAINER_RADIUS / VERLET_RADIUS + 5
#define MAX_PER_CELL 4
VerletObject* grid[DIMENSION][DIMENSION][DIMENSION][MAX_PER_CELL];

void handleGridCollision(VerletObject** currentCell, VerletObject** otherCell)
{
    for (int a = 0; currentCell[a]; a++) {
        for (int b = 0; otherCell[b]; b++) {
            VerletObject* vA = currentCell[a];
            VerletObject* vB = otherCell[b];
            if (vA != vB) {
                // printf("%p <-> %p\n", vA, vB);
                handleCollision(vA, vB);
            }
        }
    }
}

void pushNode(int gridX, int gridY, int gridZ, VerletObject* obj)
{
    VerletObject** currentCell = grid[gridX][gridY][gridZ];
    int i = 0;
    while (currentCell[i])
        i++;
    grid[gridX][gridY][gridZ][i] = obj;
}

void fillGrid(VerletObject* objects, int size)
{
    for (int i = 0; i < size; i++) {
        VerletObject* obj = &(objects[i]);
        int gridX = obj->current[0] / (VERLET_RADIUS * 2) + DIMENSION / 2;
        int gridY = obj->current[1] / (VERLET_RADIUS * 2) + DIMENSION / 2;
        int gridZ = obj->current[2] / (VERLET_RADIUS * 2) + DIMENSION / 2;
        gridX = clampi(gridX, 0, DIMENSION - 1);
        gridY = clampi(gridY, 0, DIMENSION - 1);
        gridZ = clampi(gridZ, 0, DIMENSION - 1);
        pushNode(gridX, gridY, gridZ, obj);
    }
}

void clearGrid()
{
    memset(grid, 0, DIMENSION * DIMENSION * DIMENSION * MAX_PER_CELL * sizeof(VerletObject*));
}

void* threadFunction(void* arg)
{
    int thread_id = *((int*)arg);
    int start = 1 + thread_id * ((DIMENSION) / THREAD_COUNT);
    int end = 1 + (thread_id + 1) * ((DIMENSION) / THREAD_COUNT);

    // Handle remaining iterations for the last thread
    if (thread_id == THREAD_COUNT - 1) {
        end += DIMENSION % THREAD_COUNT - 2;
    }

    // printf("%d :: %d -> %d\n", thread_id, start, end);

    for (int x = start; x < end; x++) {
        for (int y = 1; y < DIMENSION - 1; y++) {
            for (int z = 1; z < DIMENSION - 1; z++) {
                VerletObject** currentCell = grid[x][y][z];
                if (!currentCell[0])
                    continue;
                for (int dx = -1; dx <= 1; dx++) {
                    for (int dy = -1; dy <= 1; dy++) {
                        for (int dz = -1; dz <= 1; dz++) {
                            VerletObject** otherCell = grid[x + dx][y + dy][z + dz];
                            if (!otherCell[0])
                                continue;
                            handleGridCollision(currentCell, otherCell);
                        }
                    }
                }
            }
        }
    }
    return NULL;
}

pthread_t threads[THREAD_COUNT];
int thread_ids[THREAD_COUNT];

void applyGridCollisions(VerletObject* objects, int size)
{
    clearGrid();
    fillGrid(objects, size);
    // Start the threads
    for (int t = 0; t < THREAD_COUNT; t++) {
        thread_ids[t] = t;
        pthread_create(&threads[t], NULL, threadFunction, (void*)&thread_ids[t]);
    }
    // Wait for threads to finish
    for (int t = 0; t < THREAD_COUNT; t++) {
        pthread_join(threads[t], NULL);
    }
}

void applyConstraints(VerletObject* objects, int size, mfloat_t* containerPosition)
{
    // ========= Floor =========
    // for (int i = 0; i < size; i++) {
    //     VerletObject* obj = &(objects[i]);
    //     if (obj->current[1] < -2.0f) {
    //         mfloat_t disp = obj->current[1] - obj->previous[1];
    //         obj->current[1] = -2.0f;
    //         obj->previous[1] = obj->current[1] + disp;
    //     }
    // }

    // ========= Sphere =========
    // mfloat_t cRadius = CONTAINER_RADIUS;
    // // mfloat_t cPosition[VEC3_SIZE] = { 0, 0, 0 };
    // for (int i = 0; i < size; i++) {
    //     VerletObject* obj = &(objects[i]);
    //     mfloat_t disp[VEC3_SIZE];
    //     vec3_subtract(disp, obj->current, containerPosition);
    //     mfloat_t dist = vec3_length(disp);
    //     if (dist > cRadius - obj->radius) {
    //         mfloat_t norm[VEC3_SIZE];
    //         vec3_divide_f(norm, disp, dist);
    //         vec3_multiply_f(norm, norm, cRadius - obj->radius);
    //         vec3_add(obj->current, containerPosition, norm);
    //     }
    // }

    // ========= Box =========
    mfloat_t bWidth = CONTAINER_RADIUS;
    for (int i = 0; i < size; i++) {
        VerletObject* obj = &(objects[i]);

        if (obj->current[0] < -bWidth + containerPosition[0]) {
            mfloat_t disp = obj->current[0] - obj->previous[0];
            obj->current[0] = -bWidth + containerPosition[0];
            obj->previous[0] = obj->current[0] + disp;
        }
        if (obj->current[0] > bWidth + containerPosition[0]) {
            mfloat_t disp = obj->current[0] - obj->previous[0];
            obj->current[0] = bWidth + containerPosition[0];
            obj->previous[0] = obj->current[0] + disp;
        }

        if (obj->current[1] < -bWidth + containerPosition[1]) {
            mfloat_t disp = obj->current[1] - obj->previous[1];
            obj->current[1] = -bWidth + containerPosition[1];
            obj->previous[1] = obj->current[1] + disp;
        }
        if (obj->current[1] > bWidth + containerPosition[1]) {
            mfloat_t disp = obj->current[1] - obj->previous[1];
            obj->current[1] = bWidth + containerPosition[1];
            obj->previous[1] = obj->current[1] + disp;
        }

        if (obj->current[2] < -bWidth + containerPosition[2]) {
            mfloat_t disp = obj->current[2] - obj->previous[2];
            obj->current[2] = -bWidth + containerPosition[2];
            obj->previous[2] = obj->current[2] + disp;
        }
        if (obj->current[2] > bWidth + containerPosition[2]) {
            mfloat_t disp = obj->current[2] - obj->previous[2];
            obj->current[2] = bWidth + containerPosition[2];
            obj->previous[2] = obj->current[2] + disp;
        }
    }
}

void updatePositions(VerletObject* objects, int size, float dt)
{
    for (int i = 0; i < size; i++) {
        VerletObject* obj = &(objects[i]);
        mfloat_t disp[VEC3_SIZE];
        vec3_subtract(disp, obj->current, obj->previous);
        vec3_assign(obj->previous, obj->current);
        vec3_multiply_f(obj->acceleration, obj->acceleration, dt * dt);
        vec3_add(obj->current, obj->current, disp);
        vec3_add(obj->current, obj->current, obj->acceleration);
        vec3_zero(obj->acceleration);
    }
}

void addForce(VerletObject* objects, int size, mfloat_t* center, float strength)
{
    for (int i = 0; i < size; i++) {
        VerletObject* obj = &(objects[i]);
        mfloat_t disp[VEC3_SIZE];
        vec3_subtract(disp, obj->current, center);
        mfloat_t dist = vec3_length(disp);
        if (dist > 0) {
            mfloat_t norm[VEC3_SIZE];
            vec3_divide_f(norm, disp, dist);
            vec3_multiply_f(norm, norm, strength);
            vec3_add(obj->acceleration, obj->acceleration, norm);
        }
    }
}
