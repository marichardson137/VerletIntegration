#include "model.h"
#include "util.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "dependencies/include/GL/glew.h"

#define VERTEX_LIMIT 2000

DynamicArray* loadOBJ(const char* filename);
void processVertex(DynamicArray* vertices, char* vertexData[3], Vertex v[], Vertex vt[], Vertex vn[]);

Model* createModel(Mesh* mesh)
{
    Model* model = malloc(sizeof(Model));
    model->mesh = mesh;
    vec3(model->position, 0, 0, -10);
    vec3(model->rotation, 0, 0, 0);
    model->scale = 1;
    model->renderMethod = GL_TRIANGLES;
    return model;
}

void destroyModel(Model* model)
{
    destroyMesh(model->mesh);
    free(model);
}

Mesh* createMesh(const char* filename, bool instanced)
{
    DynamicArray* dArray = loadOBJ(filename);

    Mesh* mesh = malloc(sizeof(Mesh));
    mesh->vertices = dArray->array;
    mesh->numVertices = dArray->size / STRIDE;

    // Create our Vertex Buffer and Vertex Array Objects
    glGenVertexArrays(1, &(mesh->VAO));
    glBindVertexArray(mesh->VAO);

    glGenBuffers(1, &(mesh->VBO));
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).

    glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * STRIDE * mesh->numVertices, mesh->vertices, GL_STATIC_DRAW);

    // Position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, STRIDE * sizeof(float), (void*)0);
    // Normal
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, STRIDE * sizeof(float), (void*)12);
    // Texture
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, STRIDE * sizeof(float), (void*)24);

    if (instanced) {
        // Position
        glGenBuffers(1, &(mesh->positionVBO));
        glBindBuffer(GL_ARRAY_BUFFER, mesh->positionVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * INSTANCE_STRIDE * MAX_INSTANCES, NULL, GL_STREAM_DRAW);
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, INSTANCE_STRIDE * sizeof(float), (void*)0);
        glVertexAttribDivisor(3, 1);

        // Velocity
        glGenBuffers(1, &(mesh->velocityVBO));
        glBindBuffer(GL_ARRAY_BUFFER, mesh->velocityVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * MAX_INSTANCES, NULL, GL_STREAM_DRAW);
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);
        glVertexAttribDivisor(4, 1);
    }

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    // uncomment this call to draw in wireframe polygons.
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    return mesh;
}

DynamicArray* loadOBJ(const char* filename)
{
    Vertex v[VERTEX_LIMIT];
    Vertex vt[VERTEX_LIMIT];
    Vertex vn[VERTEX_LIMIT];

    int v_count = 0;
    int vt_count = 0;
    int vn_count = 0;

    DynamicArray* vertices = malloc(sizeof(DynamicArray));
    initialize(vertices, 160);

    FILE* fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error opening file: %s\n", filename);
        exit(EXIT_FAILURE);
    }

    char* line = NULL;
    size_t len = 0;
    ssize_t read;

    while ((read = getline(&line, &len, fp)) != -1) {
        char* words[4];
        words[0] = strtok(line, " ");
        for (int i = 1; i < 4; ++i) {
            words[i] = strtok(NULL, " ");
        }

        if (strcmp(words[0], "v") == 0) {
            v[v_count].x = atof(words[1]);
            v[v_count].y = atof(words[2]);
            v[v_count].z = atof(words[3]);
            v_count++;
        } else if (strcmp(words[0], "vt") == 0) {
            vt[vt_count].x = atof(words[1]);
            vt[vt_count].y = atof(words[2]);
            vt_count++;
        } else if (strcmp(words[0], "vn") == 0) {
            vn[vn_count].x = atof(words[1]);
            vn[vn_count].y = atof(words[2]);
            vn[vn_count].z = atof(words[3]);
            vn_count++;
        } else if (strcmp(words[0], "f") == 0) {
            char* v1[3];
            char* v2[3];
            char* v3[3];

            v1[0] = strtok(words[1], "/");
            v1[1] = strtok(NULL, "/");
            v1[2] = strtok(NULL, "/");
            v2[0] = strtok(words[2], "/");
            v2[1] = strtok(NULL, "/");
            v2[2] = strtok(NULL, "/");
            v3[0] = strtok(words[3], "/");
            v3[1] = strtok(NULL, "/");
            v3[2] = strtok(NULL, "/");

            processVertex(vertices, v1, v, vt, vn);
            processVertex(vertices, v2, v, vt, vn);
            processVertex(vertices, v3, v, vt, vn);
        }
    }
    fclose(fp);
    if (line) {
        free(line);
    }

    return vertices;
}

void processVertex(DynamicArray* vertices, char* vertexData[3], Vertex v[], Vertex vt[], Vertex vn[])
{
    int vertex_ptr = atoi(vertexData[0]) - 1;
    int texture_ptr = atoi(vertexData[1]) - 1;
    int normal_ptr = atoi(vertexData[2]) - 1;

    push(vertices, v[vertex_ptr].x);
    push(vertices, v[vertex_ptr].y);
    push(vertices, v[vertex_ptr].z);

    push(vertices, vn[normal_ptr].x);
    push(vertices, vn[normal_ptr].y);
    push(vertices, vn[normal_ptr].z);

    push(vertices, vt[texture_ptr].x);
    push(vertices, vt[texture_ptr].y);
}

void destroyMesh(Mesh* mesh)
{
    glDeleteVertexArrays(1, &(mesh->VAO));
    glDeleteBuffers(1, &(mesh->VBO));
    free(mesh);
}