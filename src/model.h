#ifndef __MODEL_H__
#define __MODEL_H__

#define STRIDE 8
#define INSTANCE_STRIDE 3
#define MAX_INSTANCES 20000

#include "mathc.h"

typedef struct {
    int numVertices;
    float* vertices;
    unsigned int VAO;
    unsigned int VBO;
    unsigned int positionVBO;
    unsigned int velocityVBO;
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

Mesh* createMesh(const char* filename, bool instanced);

void destroyMesh(Mesh* mesh);

#endif