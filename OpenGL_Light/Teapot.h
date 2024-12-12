#pragma once
#include <GL/glut.h>
#include <array>
#include <tuple>
class Teapot
{
private:
	GLfloat size;
	std::array<GLfloat, 3> color3f;

public:
	Teapot(GLfloat);
	~Teapot();
	Teapot& setColor(std::array<GLfloat, 3>);
	Teapot& setSize(GLfloat);
	void render_wire();
	void render_solid();
};
