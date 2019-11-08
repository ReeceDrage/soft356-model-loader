// Nupengl include statements for OpenGL rendering
#include "GL/glew.h"
#include "GL/freeglut.h"
#include "GLFW/glfw3.h"

#include "OpenGLHandler.h";

void OpenGLHandler::Initialize()
{
	// Initialise GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
}

void OpenGLHandler::CreateRenderWindow(GLFWwindow** window, int width, int height)
{
	// Generate a window and set it to the current context
	(*window) = glfwCreateWindow(width, height, "Render Window", NULL, NULL);
	glfwMakeContextCurrent(*window);

	// Initialise GLEW. Has to be after context setting to prevent errors
	glewInit();

	// Enable depth testing for camera handling.
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
}
