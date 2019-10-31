#pragma once

// Standard C/C++ include statements
#include <vector>
#include <iostream>

// Include statements for the glm maths library
#include "glm/glm.hpp"

// Struct declarations

struct FaceRecordVertex
{
	int vertexIndex;
	int textureIndex;
	int normalIndex;
};

struct FaceRecord
{
	FaceRecordVertex vertexArray[3];
};

// Class declarations

class ParserStrategy
{
public:
	virtual bool Parse(std::vector<std::string> rawData, std::vector<glm::vec4>* vertices, std::vector<glm::vec2>* textures, std::vector<glm::vec4>* normals) = 0;
};

class OBJParser : public ParserStrategy
{
public:
	bool Parse(std::vector<std::string> rawData, std::vector<glm::vec4>* vertices, std::vector<glm::vec2>* textures, std::vector<glm::vec4>* normals);

private:
	bool ParseVertex(std::string data, glm::vec4* vertex);
	bool ParseFaceData(std::string data, FaceRecord* face);
	int CalculateNumberOfLeadingCharacters(std::string data);
	void StringSplit(std::string data, std::vector<std::string>* returnedString, char delimiter);
	void RemoveLeadingCharacters(std::string* data, int numberOfCharacters);
};

class ModelParser
{
public:
	bool ParseModel(std::vector<std::string> rawData, std::vector<glm::vec4>* vertices, std::vector<glm::vec2>* textures, std::vector<glm::vec4>* normals);
	void SetStrategy(std::string input);

private:
	ParserStrategy* strategy;
};