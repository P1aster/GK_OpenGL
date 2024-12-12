#pragma once
#include <GL/glut.h>
#include <array>
#include <tuple>

class Axes
{
private:
	GLfloat size;

public:
	Axes(GLfloat);
	~Axes();
	Axes& setSize(GLfloat);
	void render();
};
