#pragma once
#include "Vertex.h"
#include <tuple>
#include <vector>
#include <GL/glut.h>
#include <iostream>
#include <numbers>
#include <random>


using namespace std;

class Egg
{
private:
	int density;
	vector<Vertex> vertices;
	int topPoleIndex;    // Index of the top pole vertex
	int bottomPoleIndex; // Index of the bottom pole vertex

	GLfloat mat_color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat mat_diffuse = 1.0f;
	GLfloat mat_specular = 1.0f;
	GLfloat mat_shininess = 20.0f;

	void generateVerticies();
	void calculateVertexPosition(GLfloat u, GLfloat v, Vertex& vetex);

public:
	Egg(int);
	Egg& setDensity(int);
	Egg& setPlainColors3f(GLfloat r, GLfloat g, GLfloat b);
	Egg& setRandomColors();
	Egg& setMaterial();
	void render_points();
	void render_triangle();
	~Egg();
};

