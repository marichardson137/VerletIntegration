#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "dependencies/include/GL/glew.h"
#include "dependencies/include/GLFW/glfw3.h"

#include "graphics.h"
#include "shader.h"
#include "model.h"
#include "verlet.h"

#define NUM_VERLET 1000
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void instantiateVerlets(VerletObject* objects, int size);

// Settings
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

int main()
{

    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Verlet Integration", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Set up a callback function for when the window is resized */
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSwapInterval(1);

    /* Initialize GLEW */
    glewInit();

    /* OpenGL Settings */
    glClearColor(0.8, 0.8, 0.8, 1.0);
    glClearStencil(0);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glPointSize(2.0);

    /* Models & Shaders */
    unsigned int shaderID = createShader("shaders/phong_vertex.glsl", "shaders/phong_fragment.glsl");

    Mesh* mesh = createMesh("models/sphere.obj");
    // Model* model = createModel(mesh);

    mfloat_t position[VEC3_SIZE] = { 0, 0, -20 };
    mfloat_t rotation[VEC3_SIZE] = { 0, 0, 0 };
    mfloat_t scale = 7;

    VerletObject* verlets = malloc(sizeof(VerletObject) * NUM_VERLET);
    instantiateVerlets(verlets, NUM_VERLET);
    int numActive = 1;
    int frameCount = 0;

    float dt = 0.000001f;
    float lastFrameTime = (float)glfwGetTime();

    char title[100] = "";

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        /* Input */
        processInput(window);

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        if (frameCount > 1 && numActive < NUM_VERLET) {
            numActive++;
            frameCount = 0;
            sprintf(title, "FPS : %-4.0f | Balls : %-10d", 1.0 / dt, numActive);
        }

        applyForces(verlets, numActive);
        applyCollisions(verlets, numActive);
        applyConstraints(verlets, numActive);
        updatePositions(verlets, numActive, dt);

        for (int i = 0; i < numActive; i++) {
            VerletObject obj = verlets[i];
            drawMesh(mesh, shaderID, GL_TRIANGLES, obj.current, rotation, obj.radius);
        }

        drawMesh(mesh, shaderID, GL_POINTS, position, rotation, scale);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();

        /* Timing */
        dt = (float)glfwGetTime() - lastFrameTime;
        lastFrameTime = (float)glfwGetTime();
        frameCount++;

        glfwSetWindowTitle(window, title);
    }

    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void instantiateVerlets(VerletObject* objects, int size)
{
    for (int i = 0; i < size; i++) {
        VerletObject* obj = &(objects[i]);
        vec3(obj->current, 2, 0, -22);
        vec3(obj->previous, 2, 0.2, -22);
        vec3(obj->acceleration, 0, 0, 0);
        obj->radius = 0.3;
    }
}
