#pragma once
#include <GL/glut.h>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <iostream>

class Texture
{
private:
	GLuint textureID;
	char* FileName;
	GLbyte* pBytes;
	GLint ImWidth, ImHeight, ImComponents;
	GLenum ImFormat;
public:
	GLbyte* LoadTGAImage(const char* FileName);
	Texture& setTexture();
	GLuint getTetureID();
	Texture& bindTexture();
};

