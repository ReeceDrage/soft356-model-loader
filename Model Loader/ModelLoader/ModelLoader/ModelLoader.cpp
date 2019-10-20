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

// Include statements for the glm maths library
#include "glm/glm.hpp"

// Include statements for external header files
#include "OBJParser.h"

using namespace std;

void ProduceRenderableModel(const ModelData* loadedModel, vector<glm::vec3>* renderableModel)
{
	// Generate vector data for rendering
	for (int i = 0; i < loadedModel->faceVector.size(); i++)
	{
		glm::vec3 vector;

		// Iterate through every face record and retrieve the corresponding vertices
		for (int j = 0; j < 3; j++)
		{
			int index = loadedModel->faceVector[i].vertexArray[j].vertexIndex - 1;

			vector.x = loadedModel->vertexVector[index].x;
			vector.y = loadedModel->vertexVector[index].y;
			vector.z = loadedModel->vertexVector[index].z;

			renderableModel->push_back(vector);
		}
	}
}

int main(int argc, char** argv)
{
	// Create input stream
	string line;
	vector<string> rawData;

	ifstream myFile("Resources/suzanne.obj");

	if (myFile.is_open())
	{
		while (getline(myFile, line))
		{
			rawData.push_back(line);
		}

		myFile.close();
	}

	OBJParser parser;
	ModelData modelData;
	vector<glm::vec3> renderableModel;

	// Parse the loaded file into readable model data
	parser.ParseOBJ(&rawData, &modelData);

	// Produce vector data, ready for rendering
	ProduceRenderableModel(&modelData, &renderableModel);

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