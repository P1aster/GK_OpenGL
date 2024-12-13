#pragma once
#include <GL/glut.h>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <iostream>

class Texture
{
public:
	GLbyte* LoadTGAImage(const char* FileName, GLint* ImWidth, GLint* ImHeight, GLint* ImComponents, GLenum* ImFormat);
};

