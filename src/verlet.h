

#ifndef __VERLET_H__
#define __VERLET_H__

#include "mathc.h"

typedef struct {
    mfloat_t current[VEC3_SIZE];
    mfloat_t previous[VEC3_SIZE];
    mfloat_t acceleration[VEC3_SIZE];
    mfloat_t radius;
} VerletObject;

void applyForces(VerletObject* objects, int size);
void applyCollisions(VerletObject* objects, int size);
void applyConstraints(VerletObject* objects, int size);
void updatePositions(VerletObject* objects, int size, float dt);

void addForce(VerletObject* objects, int size, mfloat_t* center, float strength);

#endif