#include "MouseHandler.h"

MouseHandler::MouseHandler()
    : leftMouseButtonPressed(false), rightMouseButtonPressed(false),
    xPosOld(0), yPosOld(0), deltaX(0), deltaY(0) {
}

// Callback function for mouse button events
// This function is called whenever a mouse button event occurs
// Parameters:
// - button: the button that was pressed or released (GLUT_LEFT_BUTTON, GLUT_RIGHT_BUTTON, etc.)
// - state: the state of the button (GLUT_DOWN for press, GLUT_UP for release)
// - x, y: the x and y coordinates of the mouse cursor at the time of the event
void MouseHandler::mouseCallback(int button, int state, GLsizei x, GLsizei y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        // Left mouse button pressed
        leftMouseButtonPressed = true;
        rightMouseButtonPressed = false;
        xPosOld = x;
        yPosOld = y;
    }
    else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        // Right mouse button pressed
        leftMouseButtonPressed = false;
        rightMouseButtonPressed = true;
        xPosOld = x;
        yPosOld = y;
    }
    else {
        // No mouse button pressed
        leftMouseButtonPressed = false;
        rightMouseButtonPressed = false;
    }
}

// Callback function for mouse motion events
// This function is called whenever the mouse is moved
// Parameters:
// - x, y: the new x and y coordinates of the mouse cursor
void MouseHandler::motionCallback(GLsizei x, GLsizei y) {
    // Calculate the change in mouse position
    deltaX = x - xPosOld;
    xPosOld = x;
    deltaY = y - yPosOld;
    yPosOld = y;
}

void MouseHandler::resetDelta() {
	deltaX = 0;
	deltaY = 0;
}

// Returns true if the left mouse button is currently pressed, false otherwise
bool MouseHandler::isLeftMouseButtonPressed() const {
    return leftMouseButtonPressed;
}

// Returns true if the right mouse button is currently pressed, false otherwise
bool MouseHandler::isRightMouseButtonPressed() const {
    return rightMouseButtonPressed;
}

// Returns the difference in x position since the last mouse event
int MouseHandler::getDeltaX() const {
    return deltaX;
}

// Returns the difference in y position since the last mouse event
int MouseHandler::getDeltaY() const {
    return deltaY;
}