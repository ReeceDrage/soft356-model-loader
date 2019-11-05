// Standard C/C++ include statements
#include <iostream>
#include <vector>
#include <string>

// Nupengl include statements for OpenGL rendering
#include "GL/glew.h"
#include "GL/freeglut.h"
#include "GLFW/glfw3.h"

// Include statements for the glm maths library
#include "glm/glm.hpp"

// Include custom header files
#include "RenderableModel.h"
#include "ModelLoader.h"

bool RenderableModel::LoadModel(std::string fileName)
{
	ModelLoader parser;
	std::vector<std::string> errorString;
	bool parsingSuccessful = true;

	// Attempt file loading and parsing
	try
	{
		std::cout << "Model parsing in progress. Please wait. (It can be pretty slow) \n";
		parsingSuccessful = parser.LoadModel(fileName, &model, &scaleValue);

		if (!parsingSuccessful)
		{
			throw std::exception();
		}
	}
	catch (std::exception e)
	{
		std::cout << "Model parsing failed. Please try a different file. \n";
		std::cout << "Errors: \n";
		parser.GetErrorString(&errorString);

		for (int i = 0; i < errorString.size(); i++)
		{
			std::cout << errorString[i] << "\n";
		}

		return false;
	}

	// Iterate through each model, each object and each vertex group to add face data to bufferable vectors
	for (int i = 0; i < model.objects.size(); i++)
	{
		for (int j = 0; j < model.objects[i].vertexGroups.size(); j++)
		{
			for (int k = 0; k < model.objects[i].vertexGroups[j].vertexCoordinates.size(); k++)
			{
				vertices.push_back(model.objects[i].vertexGroups[j].vertexCoordinates[k]);
				textureCoordinates.push_back(model.objects[i].vertexGroups[j].textureCoordinates[k]);
				normals.push_back(model.objects[i].vertexGroups[j].normalCoordinates[k]);
			}
		}
	}

	std::cout << "Model parsing successful! \n\n";
	return true;
}

void RenderableModel::Initialise()
{
	// Generate and bind the VAO
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	// Generate texture and set the parameters for it
	glGenTextures(1, &texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texture);

	// Generate and bind vertex, texture and colour buffers
	glGenBuffers(1, &vertexBuffer);
	glGenBuffers(1, &textureBuffer);
	glGenBuffers(1, &normalBuffer);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec4), &vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, textureBuffer);
	glBufferData(GL_ARRAY_BUFFER, textureCoordinates.size() * sizeof(glm::vec2), &textureCoordinates[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec4), &normals[0], GL_STATIC_DRAW);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// Disable vertex attributes to clear up
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glBindVertexArray(0);
}

void RenderableModel::Draw(GLuint shader, Viewport viewport, int positionOffset)
{
	// Use the previously generated VAO
	glBindVertexArray(vertexArrayObject);
	glUseProgram(shader);

	// Generate a temporary MVP value based on the main one. Used for displaying the model at an offset.
	Viewport tempViewport = viewport;
	tempViewport.TranslateModel(positionOffset, 0.0f, 0.0f);

	// Assign consistent uniform values
	glm::mat4 MVP = tempViewport.GetMVPMatrix();
	glUniformMatrix4fv(glGetUniformLocation(shader, "MVP"), 1, GL_FALSE, &MVP[0][0]);
	glUniform1f(glGetUniformLocation(shader, "scaleValue"), scaleValue);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	// Bind the generated texture
	glBindTexture(GL_TEXTURE_2D, texture);

	int offset = 0;

	for (int i = 0; i < model.objects.size(); i++)
	{
		for (int j = 0; j < model.objects[i].vertexGroups.size(); j++)
		{
			VertexGroup vertexGroup = model.objects[i].vertexGroups[j];

			if (vertexGroup.material.ambientTextureMap.texture)
			{
				TextureMap map = vertexGroup.material.ambientTextureMap;

				// Assign texture data to the bound texture
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, map.width, map.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, map.texture);
				glGenerateMipmap(GL_TEXTURE_2D);

				hasTexture = true;
			}
			else
			{
				hasTexture = false;
			}

			// Assigned uniform values that change per vertex group
			glUniform1i(glGetUniformLocation(shader, "vLoadTexture"), (int) hasTexture);
			glUniform1i(glGetUniformLocation(shader, "inputTexture"), 0);
			glUniform4fv(glGetUniformLocation(shader, "ambientColour"), 1, glm::value_ptr(vertexGroup.material.ambientColour));
			glUniform4fv(glGetUniformLocation(shader, "diffuseColour"), 1, glm::value_ptr(vertexGroup.material.diffuseColour));

			glDrawArrays(GL_TRIANGLES, offset, vertexGroup.vertexCoordinates.size());
			offset += vertexGroup.vertexCoordinates.size();
		}
	}

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	glUseProgram(0);
	glBindVertexArray(0);
}