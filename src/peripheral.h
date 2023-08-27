#ifndef __PERIPHERAL_H__
#define __PERIPHERAL_H__

#include "dependencies/include/GLFW/glfw3.h"

typedef struct {
    double xpos;
    double ypos;
    double pxpos;
    double pypos;
} Mouse;

Mouse* createMouse();
void updateMouse(GLFWwindow* window, Mouse* mouse);
double getDx(Mouse* mouse);
double getDy(Mouse* mouse);

#endif
