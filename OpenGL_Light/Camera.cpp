#include "Camera.h"

Camera::Camera() {

}

Camera::~Camera(){}

void Camera::cameraLook() {
    // Set the camera view using gluLookAt
    // viewer: Camera position in world coordinates
    // lookAt: The point in world coordinates where the camera is looking
    // upVector: The up direction for the camera
    gluLookAt(
        viewer[0], viewer[1], viewer[2],  // Camera position
        lookAt[0], lookAt[1], lookAt[2],  // Look at center
        upVector[0], upVector[1], upVector[2] // Up vector
    );
}

GLdouble Camera::clamp(GLdouble value, GLdouble min, GLdouble max) {
    // Clamp the value within the specified range [min, max]
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

Camera& Camera::updateCameraSpherePosition(GLdouble theta, GLdouble phi, GLdouble R) {
    this->theta = theta;
    this->phi = phi;
    this->R = R;
    // Update the spherical coordinates of the camera
    theta = fmod(theta, 360.0);
    phi = fmod(phi, 360.0);

    // Convert angles from degrees to radians
    GLdouble theta_rad = theta * (numbers::pi / 180.0);
    GLdouble phi_rad = phi * (numbers::pi / 180.0);

    // Keep theta in [0, 2pi) range using modulo
    theta_rad = fmod(theta_rad, 2.0 * numbers::pi);
    if (theta_rad < 0) theta_rad += 2.0 * numbers::pi;

    // Keep phi in [0, 2pi) range using modulo
    phi_rad = fmod(phi_rad, 2.0 * numbers::pi);
    if (phi_rad < 0) phi_rad += 2.0 * numbers::pi;

    // Define a small epsilon to adjust angles near critical values
    const GLdouble epsilon = 0.001;

    // Adjust theta to avoid singularities at pi/2, pi, 3pi/2
    GLdouble theta_mod = fmod(theta_rad, numbers::pi / 2.0);
    if (fabs(theta_mod) < epsilon) {
        theta_rad += epsilon;
    }

    // Adjust phi to avoid singularities at pi/2, pi, 3pi/2
    GLdouble phi_mod = fmod(phi_rad, numbers::pi / 2.0);
    if (fabs(phi_mod) < epsilon) {
        phi_rad += epsilon;
    }

    // Adjust upVector based on phi to prevent mirroring
    if (phi_rad >= 0 && phi_rad <= numbers::pi) {
        upVector[0] = 0.0;
        upVector[1] = 1.0;
        upVector[2] = 0.0;
    }
    else {
        upVector[0] = 0.0;
        upVector[1] = -1.0;
        upVector[2] = 0.0;
    }

    // Calculate camera position using spherical coordinates
    viewer[0] = R * sin(phi_rad) * cos(theta_rad);  // x
    viewer[1] = R * cos(phi_rad);                   // y
    viewer[2] = R * sin(phi_rad) * sin(theta_rad);  // z

    return *this;
}


GLdouble Camera::getPhi() {
    return phi;
};
GLdouble Camera::getTheta() {
    return theta;
};
GLdouble Camera::getR() {
    return R;
};
GLdouble Camera::getScale() {
    return scale;
};