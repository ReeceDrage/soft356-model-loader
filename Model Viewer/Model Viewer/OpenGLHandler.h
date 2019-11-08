#pragma once

// Nupengl include statements for OpenGL rendering
#include "GL/glew.h"
#include "GL/freeglut.h"
#include "GLFW/glfw3.h"

class OpenGLHandler
{
public:
	void Initialize();
	void CreateRenderWindow(GLFWwindow** window, int width, int height);
};