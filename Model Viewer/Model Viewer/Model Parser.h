#pragma once

#include "ModelStructs.h"
#include <vector>

class OBJParser
{
public:
	bool ParseOBJ(std::vector<std::string> rawData, Model model);

private:
	bool StringSplit(std::string data, std::vector<std::string>* splitData, char delimiter);
	bool ParseVertexCoordinates(std::string data, glm::vec4 vertex);
	bool ParseFaceCoordinates(std::string data, FaceRecord face);
	bool ParseTextureCoordinates(std::string data, glm::vec2 texture);
	bool ParseNormalCoordinates(std::string data, glm::vec4 normal);
};