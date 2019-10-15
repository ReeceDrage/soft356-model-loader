// Standard C/C++ include statements
#include <iostream>
#include <cstdlib>

// Nupengl include statements for OpenGL rendering
#include "GL/glew.h"
#include "GL/freeglut.h"
#include "GLFW/glfw3.h"

int main(int argc, char** argv)
{
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(1920, 1080, "Render Window", NULL, NULL);
	glfwMakeContextCurrent(window);
	glewInit();

	while (!glfwWindowShouldClose(window))
	{
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}