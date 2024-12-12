#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/glut.h>
#include <math.h>
#include <cmath>
#include <numbers>
#include <iostream>


using namespace std;

class Camera
{
private:
    GLdouble viewer[3] = { 0.0, 0.0, 5.0 }; // Viewer position
    GLdouble upVector[3] = { 0.0, 1.0, 0.0 }; // Up vector
    GLdouble lookAt[3] = { 0.0, 0.0, 0.0 }; // Look at

    GLdouble theta = 0.0;  // Rotate angle
    GLdouble phi = 0.0;  // Rotate angle
    GLdouble pix2angle = 1.0;  // Pixel to angle ratio
    GLdouble scale = 1.0;  // Scale factor
    GLdouble R = 10.0;  // Distance from the viewer

public:

    Camera();
    ~Camera();
    static  GLdouble clamp(GLdouble value, GLdouble min, GLdouble max);
    Camera& updateCameraSpherePosition(GLdouble theta, GLdouble phi, GLdouble R);
    void cameraLook();
    GLdouble getPhi();
    GLdouble getTheta();
    GLdouble getR();
    GLdouble getScale();
};
