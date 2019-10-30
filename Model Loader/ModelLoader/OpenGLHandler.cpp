// Include header file with class declarations
#include "OpenGLHandler.h"

// Initializes GLFW, GLEW and creates a window
void OpenGLHandler::Initialize(GLFWwindow** window, int height, int width)
{
	// Initialise GLFW and GLEW
	glfwInit();
	(*window) = glfwCreateWindow(width, height, "Render Window", NULL, NULL);
	glfwMakeContextCurrent(*window);
	glewInit();

	// Generate and bind the vertex array
	GLuint vertexArray;
	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);

	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);
}

// Clears the display buffer, 
void OpenGLHandler::Display(GLuint* vertexBuffer, GLuint* colourBuffer, long numberOfVertices)
{
	// Clear using depth buffer to ensure edges are rendered correctly along the Z axis
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Bind buffer values
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, (*vertexBuffer));
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// Bind buffer values
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, (*colourBuffer));
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// Draw arrays to the window
	glDrawArrays(GL_TRIANGLES, 0, numberOfVertices); // Starting from vertex 0; 3 vertices total -> 1 triangle
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}