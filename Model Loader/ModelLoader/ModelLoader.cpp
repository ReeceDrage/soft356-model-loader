// Standard C/C++ include statements
#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>
#include <vector>
#include <conio.h>
#include <locale>
#include <time.h>

// Nupengl include statements for OpenGL rendering
#include "GL/glew.h"
#include "GL/freeglut.h"
#include "GLFW/glfw3.h"

// Include statements for the glm maths library
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

// Include statements for external header files
#include "OBJParser.h"
#include "ShaderLoader.h"

using namespace std;
using namespace glm;

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

void GenerateVertexBuffer(GLuint *vertexBuffer, vector<vec4> vertices)
{
	glGenBuffers(1, vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, (*vertexBuffer));
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
}

void GenerateColourBuffer(GLuint* colourBuffer, vector<vec4> colourVector)
{
	glGenBuffers(1, colourBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, (*colourBuffer));
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * colourVector.size(), &colourVector[0], GL_STATIC_DRAW);
}

void Rotate(GLuint program, glm::vec3 rotationAxis, float rotationAngle)
{
	// Generate a rotation matrix to multiply each individual vector by
	glm::mat4 rotationMatrix = glm::rotate(rotationAngle, rotationAxis);

	GLint matrixLocation = glGetUniformLocation(program, "rotationMatrix");

	if (matrixLocation != -1)
	{
		glUniformMatrix4fv(matrixLocation, 1, GL_FALSE, glm::value_ptr(rotationMatrix));
	}
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
	glDisableVertexAttribArray(1);
}

void LoadAndParseModel(vector<vec4> *vertices, vector<vec2> *textures, vector<vec4> *normals, vector<vec4> *colourVector)
{
	vector<string> rawData;
	OBJParser parser;
	bool parsingSuccessful;

	time_t start, end;

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
		vertices->clear();
		textures->clear();
		normals->clear();
		colourVector->clear();

		time(&start);

		// Load a model file
		LoadFile(&rawData, inputString);

		// Parse the loaded file into readable model data
		parsingSuccessful = parser.ParseOBJ(rawData, vertices, textures, normals);
	} 
	while (!parsingSuccessful);

	time(&end);

	// Produce colour arrays for OpenGL to process
	GenerateRandomValues(colourVector, vertices->size());

	cout << endl << "Model Loaded. Time Taken: " << (double)end - start << "s. " << endl;
	cout << "Type QUIT to quit and LOAD to load a new model." << endl << endl;
}

int main(int argc, char** argv)
{
	// Load and parse model data
	vector<glm::vec4> vertices;
	vector<glm::vec2> textures;
	vector<glm::vec4> normals;
	vector<glm::vec4> colourVector;
	LoadAndParseModel(&vertices, &textures, &normals, &colourVector);

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

	// Load shaders
	GLuint program = LoadShaders("Resources/shader.vert", "Resources/shader.frag");

	// Genereate, bind and fill vertex and colour buffers
	GLuint vertexBuffer;
	GLuint colourBuffer;
	GenerateVertexBuffer(&vertexBuffer, vertices);
	GenerateColourBuffer(&colourBuffer, colourVector);

	// Do while loop can be exited by user input
	bool isActive = true;

	float rotationValue = 0.005f;

	do
	{
		// Generate a rotation matrix and rotate every vertex
		glm::vec3 rotationAxis(0, 1, 0);
		Rotate(program, rotationAxis, rotationValue);

		glfwSwapBuffers(window);
		glfwPollEvents();
		glUseProgram(program);
		Display(&vertexBuffer, &colourBuffer, vertices.size());

		rotationValue += 0.01f;

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
				LoadAndParseModel(&vertices, &textures, &normals, &colourVector);
				GenerateVertexBuffer(&vertexBuffer, vertices);
				GenerateColourBuffer(&colourBuffer, colourVector);
				break;
			}
		}
	} 
	while (!(glfwWindowShouldClose(window)) && isActive);
}