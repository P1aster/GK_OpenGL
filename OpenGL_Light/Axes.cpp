#include "Axes.h"


Axes::Axes(GLfloat size)
{
	this->size = size;
}

Axes::~Axes(){}

Axes& Axes::setSize(GLfloat size) {
	this->size = size;
	return *this;
}


void Axes::render()
{
    glBegin(GL_LINES);

    // X-axis (red)
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-size, 0.0f, 0.0f);
    glVertex3f(size, 0.0f, 0.0f);

    // Y-axis (green)
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, -size, 0.0f);
    glVertex3f(0.0f, size, 0.0f);

    // Z-axis (blue)
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, -size);
    glVertex3f(0.0f, 0.0f, size);

    glEnd();
	glFlush();
}