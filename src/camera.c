#include "camera.h"
#include <stdlib.h>

Camera* createCamera(mfloat_t* position)
{
    Camera* camera = malloc(sizeof(Camera));
    vec3_assign(camera->position, position);
    camera->yaw = -90;
    camera->pitch = 0;
    updateVectors(camera);
    return camera;
}

void updateVectors(Camera* camera)
{
    vec3(camera->forwards,
        MCOS(MRADIANS(camera->yaw)) * MCOS(MRADIANS(camera->pitch)),
        MSIN(MRADIANS(camera->pitch)),
        MSIN(MRADIANS(camera->yaw)) * MCOS(MRADIANS(camera->pitch)));
    vec3_normalize(camera->forwards, camera->forwards);
    vec3_cross(camera->right, camera->forwards, (mfloat_t[])GLOBAL_UP);
    vec3_normalize(camera->right, camera->right);
    vec3_cross(camera->up, camera->right, camera->forwards);
    vec3_normalize(camera->up, camera->up);
}

mfloat_t* createViewMatrix(mfloat_t* viewMatrix, Camera* camera)
{
    updateVectors(camera);
    mfloat_t target[VEC3_SIZE];
    vec3_add(target, camera->position, camera->forwards);
    mat4_look_at(viewMatrix, camera->position, target, camera->up);
    return viewMatrix;
}