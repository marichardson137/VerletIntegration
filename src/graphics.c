#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dependencies/include/GL/glew.h"

#include "graphics.h"

void drawMesh(Mesh* mesh, unsigned int shaderID, GLenum mode, mfloat_t* position, mfloat_t* rotation, mfloat_t scale)
{
    mfloat_t scaling[VEC3_SIZE] = { scale, scale, scale };

    struct {
        mfloat_t position[MAT4_SIZE];
        mfloat_t rotation[MAT4_SIZE];
        mfloat_t scaling[MAT4_SIZE];
        mfloat_t model[MAT4_SIZE];
    } matrices;

    /* Position */
    mat4_identity(matrices.position);
    mat4_translation(matrices.position, matrices.position, position);

    /* Rotation */
    mat4_identity(matrices.rotation);
    mat4_from_eulers(matrices.rotation, rotation);

    /* Scaling */
    mat4_identity(matrices.scaling);
    mat4_scaling(matrices.scaling, matrices.scaling, scaling);

    /* Model matrix */
    mat4_multiply(matrices.model, matrices.rotation, matrices.scaling);
    mat4_multiply(matrices.model, matrices.position, matrices.model);

    glUseProgram(shaderID);

    glUniformMatrix4fv(glGetUniformLocation(shaderID, "model"),
        1, GL_FALSE, matrices.model);

    mfloat_t projection[MAT4_SIZE];

    mat4_perspective(projection, to_radians(45.0), (float)1280 / (float)720, 0.1, 100.0);

    glUniformMatrix4fv(glGetUniformLocation(shaderID, "projection"),
        1, GL_FALSE, projection);

    glBindVertexArray(mesh->VAO);

    glDrawArrays(mode, 0, mesh->numVertices);

    glUseProgram(0);

    glBindVertexArray(0);
}

void drawInstanced(Mesh* mesh, unsigned int shaderID, GLenum mode, int num, mfloat_t scale)
{
    glUseProgram(shaderID);

    mfloat_t projection[MAT4_SIZE];

    mat4_perspective(projection, to_radians(45.0), (float)1280 / (float)720, 0.1, 100.0);

    glUniformMatrix4fv(glGetUniformLocation(shaderID, "projection"),
        1, GL_FALSE, projection);

    glUniform1f(glGetUniformLocation(shaderID, "scale"), scale);

    glBindVertexArray(mesh->VAO);

    glDrawArraysInstanced(mode, 0, mesh->numVertices, num);

    glUseProgram(0);

    glBindVertexArray(0);
}
