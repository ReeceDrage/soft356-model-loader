// Standard C/C++ include statements
#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>
#include <vector>

// Nupengl include statements for OpenGL rendering
#include "GL/glew.h"
#include "GL/freeglut.h"
#include "GLFW/glfw3.h"

// Include statements for external header files
#include "OBJParser.h"

using namespace std;

int main(int argc, char** argv)
{
	// Create input stream
	string line;
	vector<string> mapData;
	ifstream myFile("Resources/suzanne.obj");

	if (myFile.is_open())
	{
		while (getline(myFile, line))
		{
			mapData.push_back(line);
		}

		myFile.close();
	}

	// Test function contained in OBJParser.h, outputs a test message to the console
	TestFunction();

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