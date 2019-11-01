#pragma once

// C/C++ include statements
#include <iostream>
#include <vector>

// Include statements for the glm maths library
#include "glm/glm.hpp"

// Nupengl include statements for OpenGL rendering
#include "GL/glew.h"

using glm::vec2;
using glm::vec3;
using glm::vec4;

using std::vector;
using std::string;

struct MaterialData
{
	// Basic colouring and rendering variables
	vec3 ambientColour;
	vec3 diffuseColour;
	vec3 specularColour;
	vec3 emmissionColour;
	float opticalDensity;
	float specularColourWeight;
	float dissolveTransparency;
	GLuint illuminationModel;

	// Texture rendering variables
	char* ambientTextureMap;
	char* diffuseTextureMap;
	char* specularColourTextureMap;
	char* specularHighlightMap;
	char* alphaTextureMap;
};

struct VertexGroup
{
	// Coordinates for rendering model
	vector<vec4> vertexCoordinates;
	vector<vec2> textureCoordinates;
	vector<vec4> normalCoordinates;

	// Struct containing material data
	MaterialData material;
};

struct ModelObject
{
	vector<VertexGroup> vertexGroups;
};

struct Model
{
	vector<ModelObject> objects;
};