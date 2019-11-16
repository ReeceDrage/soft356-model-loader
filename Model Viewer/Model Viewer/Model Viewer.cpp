// Standard C/C++ include statements
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <conio.h>

// Nupengl include statements for OpenGL rendering
#include "GL/glew.h"
#include "GL/freeglut.h"
#include "GLFW/glfw3.h"

// Include statements for the glm maths library
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"

// Include custom header files
#include "ModelLoader.h"
#include "OpenGlHandler.h"
#include "ShaderLoader.h"
#include "Viewport.h"
#include "RenderableModel.h"

// Models to render
vector<RenderableModel> meshes;

std::string RequestFileSelection()
{
	std::string fileInput;

	std::cout << "Please enter a file name: \n";
	std::cin >> fileInput;

	return fileInput;
}

void LoadModel()
{
	RenderableModel model;

	// Identifies if the loaded file is renderable
	bool validFile = true;

	do
	{
		std::string filename = RequestFileSelection();
		validFile = model.LoadModel(filename);
	} 
	while (!validFile);

	// Generate renderable data and add the model to the meshes list
	model.Initialise();
	meshes.push_back(model);
}

void HandleInput(Viewport* viewport)
{
	// If a key is pressed, get the first character. If the character is P, pause the rendering and allow for command entry.
	while (_kbhit())
	{
		char inputChar = _getch();

		if (inputChar == 'p' || inputChar == 'P')
		{
			std::cout << "Type \"LOAD\" to load a new model." << std::endl;
			std::cout << "Type \"CLEAR\" to remove all models from the current display." << std::endl << std::endl;

			string command;

			std::cin >> command;

			// Convert the input string to uppercase for case-insensitivity
			for (int i = 0; i < command.size(); i++)
			{
				command[i] = toupper(command[i]);
			}

			if (command == "LOAD")
			{
				LoadModel();
			}

			else if (command == "CLEAR")
			{
				meshes.clear();
			}
		}
		else if (inputChar == 'A' || inputChar == 'a')
		{
			viewport->RotateModel(-1.0f, glm::vec3(0, 1, 0));
		}
		else if (inputChar == 'D' || inputChar == 'd')
		{
			viewport->RotateModel(1.0f, glm::vec3(0, 1, 0));
		}
	}
}

int main()
{
	// OpenGL variables
	OpenGLHandler handler;
	GLFWwindow* window;
	Viewport viewport;

	// Window size and "camera" FOV
	int windowWidth = 1000;
	int windowHeight = 1000;
	float fieldOfView = 45.0f;

	// Initialize OpenGL systems, create an MVP to view models and load shader files
	handler.Initialize();
	handler.CreateRenderWindow(&window, windowWidth, windowHeight);

	viewport.Initialize(fieldOfView, windowWidth, windowHeight);
	viewport.ViewLookAt(glm::vec3(6, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	viewport.TranslateModel(0.0f, 0.0f, 0.0f);

	GLuint shaderProgram = LoadShaders("Shaders/shader.vert", "Shaders/shader.frag");

	LoadModel();

	std::cout << "Press \"P\" to pause and enter commands. Press \"A\" or \"D\" to rotate the model.\n\n";

	do
	{
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (int i = 0; i < meshes.size(); i++)
		{
			meshes[i].Draw(shaderProgram, viewport, i);
		}

		glfwSwapBuffers(window);
		HandleInput(&viewport);
	} 
	while (!(glfwWindowShouldClose(window)));
}
