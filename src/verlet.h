

#ifndef __VERLET_H__
#define __VERLET_H__

#include "mathc.h"

#define CONTAINER_RADIUS 6.0f
#define VERLET_RADIUS 0.15f

typedef struct {
    mfloat_t current[VEC3_SIZE];
    mfloat_t previous[VEC3_SIZE];
    mfloat_t acceleration[VEC3_SIZE];
    mfloat_t radius;
} VerletObject;

struct NodeStruct {
    VerletObject* val;
    struct NodeStruct* next;
};
typedef struct NodeStruct Node;

void applyForces(VerletObject* objects, int size);
void applyCollisions(VerletObject* objects, int size);
void applyConstraints(VerletObject* objects, int size, mfloat_t* containerPosition);
void updatePositions(VerletObject* objects, int size, float dt);

void clearGrid();
void fillGrid(VerletObject* objects, int size);
void applyGridCollisions(VerletObject* objects, int size);

void addForce(VerletObject* objects, int size, mfloat_t* center, float strength);

#endif