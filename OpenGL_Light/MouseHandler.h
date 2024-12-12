#pragma once
#include <GL/glut.h>

class MouseHandler {
public:
    MouseHandler();
    void mouseCallback(int button, int state, GLsizei x, GLsizei y);
    void motionCallback(GLsizei x, GLsizei y);
    bool isLeftMouseButtonPressed() const;
    bool isRightMouseButtonPressed() const;
    int getDeltaX() const;
    int getDeltaY() const;
    void resetDelta();

private:
    bool leftMouseButtonPressed;
    bool rightMouseButtonPressed;
    int xPosOld;
    int yPosOld;
    int deltaX;
    int deltaY;
};