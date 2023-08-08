#ifndef __MODEL_H__
#define __MODEL_H__

#define STRIDE 8

#include "mathc.h"

typedef struct {
    int numVertices;
    float* vertices;
    unsigned int VAO;
    unsigned int VBO;
} Mesh;

typedef struct {
    Mesh* mesh;
    mfloat_t position[VEC3_SIZE];
    mfloat_t rotation[VEC3_SIZE];
    mfloat_t scale;
    unsigned int renderMethod;
} Model;

typedef struct {
    float x, y, z;
} Vertex;

Model* createModel(Mesh* mesh);

void destroyModel(Model* model);

Mesh* createMesh(const char* filename);

void destroyMesh(Mesh* mesh);

#endif