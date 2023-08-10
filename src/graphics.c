#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dependencies/include/GL/glew.h"

#include "graphics.h"

void drawMesh(Mesh* mesh, unsigned int shaderID, mfloat_t* position, mfloat_t* rotation, mfloat_t scale)
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

    printf("\n");
    for (int i = 0; i < MAT4_SIZE; i += 4) {
        printf("%.1f %.1f %.1f %.1f\n", matrices.rotation[i], matrices.rotation[i + 1],
            matrices.rotation[i + 2], matrices.rotation[i + 3]);
    }

    /* Scaling */
    mat4_identity(matrices.scaling);
    mat4_translation(matrices.scaling, matrices.scaling, scaling);

    /* Model matrix */
    mat4_multiply(matrices.model, matrices.scaling, matrices.rotation);
    mat4_multiply(matrices.model, matrices.position, matrices.model);

    glUseProgram(shaderID);

    glUniformMatrix4fv(glGetUniformLocation(shaderID, "model"),
        1, GL_FALSE, matrices.model);

    mfloat_t projection[MAT4_SIZE];

    mat4_perspective(projection, to_radians(45.0), (float)1400 / (float)900, 0.1, 100.0);

    glUniformMatrix4fv(glGetUniformLocation(shaderID, "projection"),
        1, GL_FALSE, projection);

    glBindVertexArray(mesh->VAO);

    glDrawArrays(GL_TRIANGLES, 0, mesh->numVertices);

    glUseProgram(0);

    glBindVertexArray(0);
}
