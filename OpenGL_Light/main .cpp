#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <GL/glut.h>
#include <math.h>
#include <tuple>
#include "Egg.h"
#include "Axes.h"
#include "Teapot.h"
#include "Camera.h"
#include "MouseHandler.h"
#include "Light.h"

using namespace std;

Axes axes(5);
Egg egg(10);
Teapot teapot(2.0);
Camera camera;
Light light_1(GL_LIGHT0);
Light light_2(GL_LIGHT1);

//Light light_2;
MouseHandler mouseHandler;

GLdouble ZNear = 0.01f;
GLdouble ZFar = 40.0f;

static float angles[] = { 0.0, 0.0, 0.0 }; //Camera position
int model = 1; // 1 - points, 2 - lines, 3 - triangles, 4 - teapot-wire, 5 - teapot-solid
int mode = 1; // 1 - camera rotation, 2 - light_1 rotation, 3 - light_2 rotation
int verticesNumber; // Number of vertices
GLdouble pix2angle = 1.0;  // Pixel to angle ratio

GLfloat scale = 1.0;  // Scale factor
GLdouble theta = 0.0;  // Rotate angle
GLdouble phi = 0.0;  // Rotate angle
GLdouble R = 10.0;  // Distance from the viewer


// Function to update viewport
void update_viewport(int horizontal, int vertical) {
    // Sets the aspect ratio and perspective of the camera based on the window size
    pix2angle = 360.0 / (GLdouble)horizontal; // Pixel to angle ratio

    glMatrixMode(GL_PROJECTION); // Set the projection matrix as the active matrix
    glLoadIdentity(); // Reset the projection matrix


    // Set the camera perspective
    // gluPerspective(fovY, aspect, zNear, zFar)
    // fovY: Field of view angle in the y direction (in degrees)
    // aspect: Aspect ratio of the window (width/height)
    // zNear: Distance to the near clipping plane
    // zFar: Distance to the far clipping plane
    gluPerspective(70, horizontal / vertical, ZNear, ZFar);


    // Set the viewport to maintain the aspect ratio of the image
    if (horizontal <= vertical)
        glViewport(0, 0, horizontal, horizontal); // Square viewport, fit to width
    else
        glViewport((horizontal - vertical) / 2, 0, vertical, vertical); // Square viewport, fit to height

    glMatrixMode(GL_MODELVIEW); // Set the model/view matrix as the active matrix
    glLoadIdentity(); // Reset the model/view matrix

}

void render() {
    // Clear the color buffer and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();



    // Update camera angles based on mouse movement
    switch (mode) {
    case 1: 
        if (mouseHandler.isLeftMouseButtonPressed()) {
            theta = camera.getTheta() + mouseHandler.getDeltaX() * pix2angle;
            phi = camera.getPhi() + mouseHandler.getDeltaY() * pix2angle;
        }
        else if (mouseHandler.isRightMouseButtonPressed()) {
            R = Camera::clamp(camera.getR() + mouseHandler.getDeltaY() * pix2angle, ZNear, ZFar);
        }
        camera.updateCameraSpherePosition(theta, phi, R);

        break;
    
    case 2: 
        if (mouseHandler.isLeftMouseButtonPressed()) {
            theta = light_1.getTheta() + mouseHandler.getDeltaX() * pix2angle;
            phi = light_1.getPhi() + mouseHandler.getDeltaY() * pix2angle;
        }
        else if (mouseHandler.isRightMouseButtonPressed()) {
            R = light_1.getR() + mouseHandler.getDeltaY() * pix2angle;
        }
        light_1.updateLightSpherePosition(theta, phi, R);
        break;
    
    case 3: 
        if (mouseHandler.isLeftMouseButtonPressed()) {
            theta = light_2.getTheta() + mouseHandler.getDeltaX() * pix2angle;
            phi = light_2.getPhi() + mouseHandler.getDeltaY() * pix2angle;
        }
        else if (mouseHandler.isRightMouseButtonPressed()) {
            R = light_2.getR() + mouseHandler.getDeltaY() * pix2angle;
        }
        light_2.updateLightSpherePosition(theta, phi, R);
        break;
    
    }


    camera.cameraLook(); // Apply camera transformations

    // Set light transformations
    light_1.setLightPosition().renderCone();
    light_2.setLightPosition().renderCone();

    axes.render();
    glScalef(scale, scale, scale);
    glRotatef(angles[0], 1, 0, 0);
    glRotatef(angles[1], 0, 1, 0);
    glRotatef(angles[2], 0, 0, 1);

    // Render the selected model
    switch (model) {
    case 1:
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
        egg.render_points();
        break;
    case 2:
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        egg.render_triangle();
        break;
    case 3:
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        egg.render_triangle();
        break;
    case 4:
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        teapot.render_wire();
        break;
    case 5:
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        teapot.render_solid();
        break;
    }

    glFlush();
    // Swap the front and back buffers
    glutSwapBuffers();
}

void keyboard_keys(unsigned char key, GLsizei x, GLsizei y)
{
    switch (key) {
    case 'u':
		mode = 1;
		break;
    case 'i':
        mode = 2;
        break;
    case 'o':
        mode = 3;
        break;
    case '1':
        model = 1;
        break;
    case '2':
        model = 2;
        break;
    case '3':
        model = 3;
        break;
    case '4':
        model = 4;
        break;
    case '5':
        model = 5;
        break;
    case 'w':
        angles[0] += 5.0;
        break;
    case 's':
        angles[0] -= 5.0;
        break;
    case 'a':
        angles[1] += 5.0;
        break;
    case 'd':
        angles[1] -= 5.0;
        break;
    case 'q':
        angles[2] -= 5.0;
        break;
    case 'e':
        angles[2] += 5.0;
        break;
    default:
        return;
    }

    render();
}

void mouse_callback(int button, int state, GLsizei x, GLsizei y) {
    mouseHandler.mouseCallback(button, state, x, y);
}
void motion_callback(GLsizei x, GLsizei y) {
    mouseHandler.motionCallback(x, y);
    glutPostRedisplay();

}

void init_menu() {
    cout << "OpenGL LAB 5" << endl;
    cout << "Controls:" << endl;
    cout << "[a]: rotate -X axis" << endl;
    cout << "[d]: rotate +X axis" << endl;
    cout << "[w]: rotate +Y axis" << endl;
    cout << "[s]: rotate -Y axis" << endl;
    cout << "[q]: rotate -Z axis" << endl;
    cout << "[e]: rotate +Z axis" << endl;
    cout << "_______________________________" << endl;
    cout << "[1]: draw points" << endl;
    cout << "[2]: draw lines" << endl;
    cout << "[3]: draw triangles" << endl;
    cout << "[4]: draw teapot wire" << endl;
    cout << "[5]: draw teapot solid" << endl;
    cout << "_______________________________" << endl;
    cout << "[u] rotate camera" << endl;
    cout << "[i] rotate light 1" << endl;
    cout << "[o] rotate light 2" << endl;
    cout << "_______________________________" << endl;
    cout << "Enter number of vertices: ";
    cin >> verticesNumber;
    egg.setDensity(verticesNumber);

}

// Initialize OpenGL settings and scene objects
void init() {

    // Set the clear color for the color buffer to a dark gray shade (RGBA: 0.1, 0.1, 0.1, 1.0)
    // This color will be used when clearing the color buffer with glClear
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    // Assign random colors to the vertices of the Egg object to enhance visual diversity
    egg.setRandomColors();

    // Configure the material properties of the Egg object, including ambient, diffuse, and specular components
    egg.setMaterial();

    // Set up the first light source (light_1) with red color (RGBA: 1.0, 0.0, 0.0, 1.0)
    // Apply lighting parameters such as position, attenuation, and spotlight attributes
    light_1.setColor(1.0f, 0.0f, 0.0f, 1.0f).setLightOptions();

    // Set up the second light source (light_2) with blue color (RGBA: 0.0, 0.0, 1.0, 1.0)
    // Apply lighting parameters similarly to light_1
    light_2.setColor(0.0f, 0.0f, 1.0f, 1.0f).setLightOptions();

    // Enable lighting calculations in OpenGL, allowing the scene to be lit by defined light sources
    glEnable(GL_LIGHTING);

    // Activate the first and second light sources so they contribute to the lighting of the scene
    light_1.enableLight();
    light_2.enableLight();

    // Enable depth testing to ensure correct rendering of objects based on their distance from the camera
    // This prevents closer objects from being obscured by farther ones
    glEnable(GL_DEPTH_TEST);

    // Set the shading model to smooth (Gouraud shading), which interpolates vertex colors across polygons
    glShadeModel(GL_SMOOTH);
}

int main(int argc, char** argv) {

    init_menu();

    //Initialize GLUT
    glutInit(&argc, argv);

    /*
        Initialize GLUT window mode. We It opens a window with the specified dimensions and properties
        such a double buffer mode, RGB color palette and enabled depth buffor.
    */
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(600, 600);

    // create the window 
    int window = glutCreateWindow("OpenGL LAB 4");

    glutDisplayFunc(render);
    glutReshapeFunc(update_viewport);

    init();

    /*
        glutKeyboardFunc setup an callback function to be called when a standart key
        (like letters, numbers, or basic symbols) is pressed on the keyboard.
    */
    glutKeyboardFunc(keyboard_keys);

    glutMouseFunc(mouse_callback);
    glutMotionFunc(motion_callback);

    glEnable(GL_DEPTH_TEST);


    // Enter the GLUT event processing loop
    glutMainLoop();
    return 0;
}