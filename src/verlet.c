#include "verlet.h"

#define GRAVITY -100.0f

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
            handleCollision(&objects[a], &objects[b]);
        }
    }
}

void applyConstraints(VerletObject* objects, int size)
{
}

void updatePositions(VerletObject* objects, int size, float dt)
{
    for (int i = 0; i < size; i++) {
        VerletObject obj = objects[i];
        mfloat_t disp[VEC3_SIZE];
        vec3_subtract(disp, obj.current, obj.previous);
        vec3_assign(obj.previous, obj.current);
        vec3_multiply_f(obj.acceleration, obj.acceleration, dt * dt);
        vec3_add(obj.current, obj.current, disp);
        vec3_add(obj.current, obj.current, obj.acceleration);
        vec3_zero(obj.acceleration);
    }
}
