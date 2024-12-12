#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/glut.h>
#include <math.h>
#include <cmath>
#include <numbers>
#include <iostream>

using namespace std;


class Light
{
private:
	GLenum lightId = GL_LIGHT0; // Default light identifier

    GLdouble theta = 0.0;  // Rotate angle
    GLdouble phi = 90.0;  // Rotate angle
    GLdouble R = 10.0;  // Distance

	GLfloat light_position[4] = { 5.0f, 0.0f, 15.0f, 1.0f }; 

	GLfloat light_color[4] = { 0.0f, 0.0f, 1.0f, 1.0f };
	GLfloat light_diffuse = 1.0f;
	GLfloat light_specular = 1.0f;

	GLfloat att_constant = 1.0;
	GLfloat att_linear = 0.05;
	GLfloat att_quadratic = 0.001;

	GLfloat spot_direction[3] = {1.0f, 0.0f, 0.0f};
	GLfloat spot_cutoff = 50.0f;
	GLfloat spot_exponent = 25.0f;


public:
	Light(GLenum lightId);
	~Light();

	Light& setLightPosition();
	Light& setLightOptions();
	void renderCone();
	void enableLight();
	Light& updateLightSpherePosition(GLdouble theta, GLdouble phi, GLdouble R);
	
	Light& setSpotDirection(GLfloat x, GLfloat y, GLfloat z);
	Light& setSpotCutoff(GLfloat cutoff);
	Light& setSpotExponent(GLfloat exponent);

	Light& setDiffuse(GLfloat diffuse);
	Light& setSpecular(GLfloat specular);
	Light& setAttenuation(GLfloat constant, GLfloat linear, GLfloat quadratic);
	Light& setColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);

	GLdouble getPhi();
	GLdouble getTheta();
	GLdouble getR();
};
