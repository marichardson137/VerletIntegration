#include "peripheral.h"

#include <stdlib.h>

Mouse* createMouse()
{
    Mouse* mouse = malloc(sizeof(Mouse));
    mouse->xpos = 0;
    mouse->ypos = 0;
    mouse->pxpos = 0;
    mouse->pypos = 0;
    return mouse;
}

void updateMouse(GLFWwindow* window, Mouse* mouse)
{
    mouse->pxpos = mouse->xpos;
    mouse->pypos = mouse->ypos;
    glfwGetCursorPos(window, &(mouse->xpos), &(mouse->ypos));
}

double getDx(Mouse* mouse)
{
    return mouse->xpos - mouse->pxpos;
}

double getDy(Mouse* mouse)
{
    return mouse->ypos - mouse->pypos;
}
