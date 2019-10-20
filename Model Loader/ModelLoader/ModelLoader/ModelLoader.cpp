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
#include "glm/gtx/transform.hpp"
#include <glm/gtc/matrix_transform.hpp>

// Include statements for external header files
#include "OBJParser.h"

using namespace std;

void ProduceRenderableModel(const ModelData* loadedModel, vector<glm::vec4>* renderableModel)
{
	// Generate vector data for rendering
	for (int i = 0; i < loadedModel->faceVector.size(); i++)
	{
		glm::vec4 vector;

		// Iterate through every face record and retrieve the corresponding vertices
		for (int j = 0; j < 3; j++)
		{
			int index = loadedModel->faceVector[i].vertexArray[j].vertexIndex - 1;

			vector.x = loadedModel->vertexVector[index].x;
			vector.y = loadedModel->vertexVector[index].y;
			vector.z = loadedModel->vertexVector[index].z;
			vector.w = loadedModel->vertexVector[index].w;

			renderableModel->push_back(vector);
		}
	}
}

void Rotate(vector<glm::vec4>* model, glm::vec3 rotationAxis, float rotationAngle)
{
	// Vector to store modified vertex data
	vector<glm::vec4> adjustedModel;

	// Generate a rotation matrix to multiply each individual vector by
	glm::mat4 rotationMatrix = glm::rotate(rotationAngle, rotationAxis);
	glm::vec4 adjustedVector;

	// For every vertex, multiply by the rotation matrix and then add the result to the new vector
	for (int i = 0; i < model->size(); i++)
	{
		adjustedVector = rotationMatrix * (*model)[i];
		adjustedModel.push_back(adjustedVector);
	}

	// replace the original vertex vector with the adjusted vertex vector
	(*model) = adjustedModel;
	adjustedModel.clear();
}

void Display(GLuint* vertexbuffer, int numberOfVertices)
{
	// Clear the buffer with black
	const float black[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	glClearBufferfv(GL_COLOR, 0, black);

	// Bind buffer values
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, (*vertexbuffer));
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (void*) 0);

	// Draw arrays to the window
	glDrawArrays(GL_TRIANGLES, 0, numberOfVertices); // Starting from vertex 0; 3 vertices total -> 1 triangle
	glDisableVertexAttribArray(0);

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
	vector<glm::vec4> renderableModel;

	// Parse the loaded file into readable model data
	parser.ParseOBJ(&rawData, &modelData);

	// Produce vector data, ready for rendering
	ProduceRenderableModel(&modelData, &renderableModel);

	// Initialise GLFW and GLEW
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(800, 600, "Render Window", NULL, NULL);
	glfwMakeContextCurrent(window);
	glewInit();

	GLuint vertexArray;
	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);

	while (!glfwWindowShouldClose(window))
	{
		glm::vec3 rotationAxis(0, 1, 0);
		Rotate(&renderableModel, rotationAxis, 0.005f);

		GLuint vertexbuffer;
		glGenBuffers(1, &vertexbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * renderableModel.size(), &renderableModel[0], GL_STATIC_DRAW);

		glfwSwapBuffers(window);
		glfwPollEvents();
		Display(&vertexbuffer, renderableModel.size());
	}
}