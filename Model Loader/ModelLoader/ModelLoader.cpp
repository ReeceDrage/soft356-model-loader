// Standard C/C++ include statements
#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>
#include <vector>
#include <conio.h>
#include <locale>

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
#include "ShaderLoader.h"

using namespace std;

void LoadFile(vector<string>* data, string fileName)
{
	// Create input stream
	string line;
	ifstream myFile(fileName);

	// If the file is open, retrieve lines from the input stream
	if (myFile.is_open())
	{
		while (getline(myFile, line))
		{
			data->push_back(line);
		}

		myFile.close();
	}
}

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

void GenerateRandomValues(vector<glm::vec4>* outputVector, int numberToGenerate)
{
	vector<glm::vec4> generatedVector;

	for (int i = 0; i < numberToGenerate; i++)
	{
		glm::vec4 colourVec;

		colourVec.x = (float) (rand() % 10) / 10;	// Red
		colourVec.y = (float) (rand() % 10) / 10;	// Green
		colourVec.z = (float) (rand() % 10) / 10;	// Blue
		colourVec.w = (float) (rand() % 10) / 10;	// Alpha

		generatedVector.push_back(colourVec);
	}

	(*outputVector) = generatedVector;
	generatedVector.clear();
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

void Display(GLuint* vertexBuffer, GLuint* colourBuffer, long numberOfVertices)
{
	// Clear using depth buffer to ensure edges are rendered correctly along the Z axis
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Bind buffer values
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, (*vertexBuffer));
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (void*) 0);

	// Bind buffer values
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, (*colourBuffer));
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// Draw arrays to the window
	glDrawArrays(GL_TRIANGLES, 0, numberOfVertices); // Starting from vertex 0; 3 vertices total -> 1 triangle
	glDisableVertexAttribArray(0);
}

void LoadAndParseModel(vector<glm::vec4> *renderableModel, vector<glm::vec4> *colourVector)
{
	vector<string> rawData;
	ModelData modelData;
	OBJParser parser;
	bool parsingSuccessful;

	// Attempt loading until a valid model is loaded
	do
	{
		// Request that the user specifies a model location
		string inputString;
		cout << "Please specify a model file location" << endl;
		cin >> inputString;
		cout << endl;

		// Clear any already loaded data
		rawData.clear();
		renderableModel->clear();
		colourVector->clear();

		// Load a model file
		LoadFile(&rawData, inputString);

		// Parse the loaded file into readable model data
		parsingSuccessful = parser.ParseOBJ(&rawData, &modelData);
	} 
	while (!parsingSuccessful);

	// Produce final renderable vector arrays for OpenGL to process
	ProduceRenderableModel(&modelData, renderableModel);
	GenerateRandomValues(colourVector, renderableModel->size());
}

int main(int argc, char** argv)
{
	vector<glm::vec4> renderableModel;
	vector<glm::vec4> colourVector;

	LoadAndParseModel(&renderableModel, &colourVector);

	// Initialise GLFW and GLEW
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(800, 800, "Render Window", NULL, NULL);
	glfwMakeContextCurrent(window);
	glewInit();

	GLuint vertexArray;
	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	GLuint program = LoadShaders("Resources/shader.vert", "Resources/shader.frag");

	GLuint vertexbuffer;
	GLuint colourbuffer;

	// Do while loop can be exited by user input
	cout << endl << "Model Loaded. Press Q to quit." << endl;
	bool isActive = true;

	do
	{
		// Generate a rotation matrix and rotate every vertex
		glm::vec3 rotationAxis(0, 1, 0);
		Rotate(&renderableModel, rotationAxis, 0.005f);

		// Re-generate, re-bind and re-fill buffers with new vertex data 
		glGenBuffers(1, &vertexbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * renderableModel.size(), &renderableModel[0], GL_STATIC_DRAW);

		glGenBuffers(1, &colourbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, colourbuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * colourVector.size(), &colourVector[0], GL_STATIC_DRAW);

		glfwSwapBuffers(window);
		glfwPollEvents();
		glUseProgram(program);
		Display(&vertexbuffer, &colourbuffer, renderableModel.size());

		// If a key is pressed, pause rendering and allow the user it enter a full line
		while (_kbhit())
		{
			string userInput;
			string capitalisedInput;

			// Take user input
			cin >> userInput;

			// Capitalise the input to remove case sensitivity
			for (int i = 0; i < userInput.length(); i++)
			{
				capitalisedInput.push_back(toupper(userInput[i]));
			}

			if (capitalisedInput == "QUIT")
			{
				isActive = false;
				break;
			}

			if (capitalisedInput == "LOAD")
			{
				LoadAndParseModel(&renderableModel, &colourVector);
				break;
			}
		}
	} 
	while (!(glfwWindowShouldClose(window)) && isActive);
}