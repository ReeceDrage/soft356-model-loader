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

struct FaceRecordVertex
{
	int vertexIndex;
	int textureIndex;
	int normalIndex;
};

struct FaceRecord
{
	vector<FaceRecordVertex> vertexVector;
};

struct TextureMap
{
	unsigned char* texture;

	GLint height;
	GLint width;
	GLint channelCount;
};

struct MaterialData
{
	std::string materialName;

	// Basic colouring and rendering variables
	vec4 ambientColour;
	vec4 diffuseColour;
	vec4 specularColour;
	vec4 emissionColour;
	float opticalDensity;
	float specularExponent;
	float dissolveTransparency;
	GLuint illuminationModel;

	// Texture rendering variables
	TextureMap ambientTextureMap;
	TextureMap diffuseTextureMap;
	TextureMap specularColourTextureMap;
	TextureMap specularHighlightMap;
	TextureMap alphaTextureMap;
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