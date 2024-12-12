#include "Teapot.h"

Teapot::Teapot(GLfloat size) {
	this->size = size;
	this->color3f = {1.0f, 1.0f, 1.0f };
}
	
Teapot::~Teapot(){}

Teapot& Teapot::setColor(std::array<GLfloat, 3> color3f)
{
	this->color3f = color3f;
	return *this;
}

Teapot& Teapot::setSize(GLfloat size)
{
	this->size = size;
	return *this;
}

void Teapot::render_wire()
{
    glColor3f(color3f[0], color3f[1], color3f[2]);
    glutWireTeapot(size);
}

void Teapot::render_solid()
{
	glColor3f(color3f[0], color3f[1], color3f[2]);
	glutSolidTeapot(size);
}