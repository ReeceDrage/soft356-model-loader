#pragma once

// Standard C/C++ include statements
#include <iostream>
#include <vector>
#include <string>

#include "glm/glm.hpp"

// Include custom header files
#include "ModelStructs.h"
#include "Viewport.h"

class RenderableModel
{
public:

	Model model;

	void Initialise();
	bool LoadModel(std::string fileName);
	void Draw(GLuint shader, Viewport viewport, int positionOffset);

private:

	// Array object record
	GLuint vertexArrayObject;

	// Buffer records
	GLuint vertexBuffer;
	GLuint textureBuffer;
	GLuint normalBuffer;
	GLuint ambientColourBuffer;
	GLuint texture;

	// Values used to modify rendering
	bool hasTexture;
	float scaleValue = 1.0f;

	// Vectors of values for rendering
	std::vector<glm::vec4> vertices;
	std::vector<glm::vec2> textureCoordinates;
	std::vector<glm::vec4> normals;
	std::vector<glm::vec4> ambientColours;
};
