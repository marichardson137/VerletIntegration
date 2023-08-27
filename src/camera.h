#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "mathc.h"

#define GLOBAL_UP        \
    {                    \
        0.0f, 1.0f, 0.0f \
    }

typedef struct {
    mfloat_t position[VEC3_SIZE];
    mfloat_t yaw;
    mfloat_t pitch;
    mfloat_t forwards[VEC3_SIZE];
    mfloat_t right[VEC3_SIZE];
    mfloat_t up[VEC3_SIZE];
} Camera;

Camera* createCamera(mfloat_t* position);
void updateVectors(Camera* camera);
mfloat_t* createViewMatrix(mfloat_t* viewMatrix, Camera* camera);

#endif