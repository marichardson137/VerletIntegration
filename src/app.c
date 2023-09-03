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
#include "camera.h"
#include "peripheral.h"

#define NUM_VERLET 5000
#define ANIMATION_TIME 90.0f // Frames
#define TARGET_FPS 100

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void cursor_enter_callback(GLFWwindow* window, int entered);
void processInput(GLFWwindow* window);
void updateCamera(GLFWwindow* window, Mouse* mouse, Camera* camera);
void instantiateVerlets(VerletObject* objects, int size);

// Settings
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

bool cursorEntered = false;
Camera* camera;
float cameraRadius = 20.0f;
int totalFrames = 0;

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

    /* Callback function for mouse enter/exit */
    glfwSetCursorEnterCallback(window, cursor_enter_callback);

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

    // Container
    mfloat_t position[VEC3_SIZE] = { 0, 0, 0 };
    mfloat_t rotation[VEC3_SIZE] = { 0, 0, 0 };
    mfloat_t scale = 7;

    VerletObject* verlets = malloc(sizeof(VerletObject) * NUM_VERLET);
    instantiateVerlets(verlets, NUM_VERLET);
    int numActive = 0;

    mfloat_t view[MAT4_SIZE];
    camera = createCamera((mfloat_t[]) { 0, 0, cameraRadius });

    Mouse* mouse = createMouse();

    float dt = 0.000001f;
    float lastFrameTime = (float)glfwGetTime();

    char title[100] = "";

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        /* Input */
        updateMouse(window, mouse);
        processInput(window);

        if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
            addForce(verlets, numActive, (mfloat_t[]) { 0, 3, 0 }, -50.0f);
        }

        /* Camera */
        updateCamera(window, mouse, camera);

        glUseProgram(shaderID);
        createViewMatrix(view, camera);
        glUniformMatrix4fv(glGetUniformLocation(shaderID, "view"),
            1, GL_FALSE, view);
        glUseProgram(0);

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        if (1.0 / dt >= 95 && glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS && numActive < NUM_VERLET) {
            numActive++;
        }

        if (totalFrames % 60 == 0) {
            sprintf(title, "FPS : %-4.0f | Balls : %-10d", 1.0 / dt, numActive);
            glfwSetWindowTitle(window, title);
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
        while (dt < 1.0f / TARGET_FPS) {
            dt = (float)glfwGetTime() - lastFrameTime;
        }
        lastFrameTime = (float)glfwGetTime();
        totalFrames++;
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

void updateCamera(GLFWwindow* window, Mouse* mouse, Camera* camera)
{

    float speed = 0.08f;
    mfloat_t temp[VEC3_SIZE];

    float universalAngle = totalFrames / 4.0f;
    vec3(camera->position, MCOS(MRADIANS(universalAngle)) * cameraRadius, camera->position[1], MSIN(MRADIANS(universalAngle)) * cameraRadius);
    camera->yaw = universalAngle + 180.0f;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        vec3_add(camera->position, camera->position, vec3_multiply_f(temp, camera->up, speed));
        camera->pitch -= 0.2f;
        cameraRadius -= 0.01f;
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        vec3_subtract(camera->position, camera->position, vec3_multiply_f(temp, camera->up, speed));
        camera->pitch += 0.2f;
        cameraRadius += 0.01f;
    }

    // if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    //     vec3_add(camera->position, camera->position, vec3_multiply_f(temp, camera->up, speed));

    // if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    //     vec3_subtract(camera->position, camera->position, vec3_multiply_f(temp, camera->up, speed));

    // // Mouse Movement
    // if (cursorEntered) {

    //     double dx = 0.1 * getDx(mouse);
    //     double dy = 0.1 * getDy(mouse);
    //     camera->yaw += dx;

    //     camera->pitch -= dy;
    //     if (camera->pitch > 89.0f) {
    //         camera->pitch = 89.0f;
    //     }
    //     if (camera->pitch < -89.0f) {
    //         camera->pitch = -89.0f;
    //     }
    // }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void cursor_enter_callback(GLFWwindow* window, int entered)
{
    if (entered) {
        // The cursor entered the content area of the window
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        cursorEntered = true;
    } else {
        // The cursor left the content area of the window
    }
}

void instantiateVerlets(VerletObject* objects, int size)
{
    for (int i = 0; i < size; i++) {
        VerletObject* obj = &(objects[i]);
        vec3(obj->current, 2, 0, -2);
        vec3(obj->previous, 2.1, -0.2, -2.2);
        vec3(obj->acceleration, 0, 0, 0);
        obj->radius = 0.25;
    }
}
