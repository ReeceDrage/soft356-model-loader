#pragma once

// Nupengl include statements for OpenGL rendering
#include "GL/glew.h"
#include "GL/freeglut.h"
#include "GLFW/glfw3.h"

class OpenGLHandler
{
public:
	void Initialize(GLFWwindow** window, int height, int width);
	void Display(GLuint* vertexBuffer, GLuint* colourBuffer, long numberOfVertices);
};