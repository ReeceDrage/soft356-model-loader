#pragma once

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

class OBJParser
{
private:

	int CalculateNumberOfLeadingCharacters(std::string data);
	void RemoveLeadingCharacters(std::string* data, int numberOfCharacters);
	void StringSplit(std::string data, std::vector<std::string>* returnedString, char delimiter);

	bool ParseVertex(std::string data, glm::vec4* vertex);
	bool ParseFaceData(std::string data, FaceRecord* face);

public:

	bool ParseOBJ(std::vector<std::string> rawData, std::vector<glm::vec4>* vertices, std::vector<glm::vec2>* textures, std::vector<glm::vec4>* normals);
};