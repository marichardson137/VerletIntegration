#include "verlet.h"

#include <stdio.h>

#define GRAVITY -15.0f

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

void applyConstraints(VerletObject* objects, int size)
{
    // Floor
    // for (int i = 0; i < size; i++) {
    //     VerletObject* obj = &(objects[i]);
    //     if (obj->current[1] < -2.0f) {
    //         mfloat_t disp = obj->current[1] - obj->previous[1];
    //         obj->current[1] = -2.0f;
    //         obj->previous[1] = obj->current[1] + disp;
    //     }
    // }

    // Circle

    mfloat_t cRadius = 7;
    mfloat_t cPosition[VEC3_SIZE] = { 0, 0, 0 };

    for (int i = 0; i < size; i++) {
        VerletObject* obj = &(objects[i]);
        mfloat_t disp[VEC3_SIZE];
        vec3_subtract(disp, obj->current, cPosition);
        mfloat_t dist = vec3_length(disp);
        if (dist > cRadius - obj->radius) {
            mfloat_t norm[VEC3_SIZE];
            vec3_divide_f(norm, disp, dist);
            vec3_multiply_f(norm, norm, cRadius - obj->radius);
            vec3_add(obj->current, cPosition, norm);
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
