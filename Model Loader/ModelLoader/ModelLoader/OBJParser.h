#pragma once

// Holds coordinates of individual vertices
struct VertexRecord
{
	float x;
	float y;
	float z;
	float w;
};

// Holds indices of data of an individual vertex on a face
struct FaceRecordVertex
{
	int vertexIndex;
	int textureIndex;
	int normalIndex;
};

// Holds records of each vertex linked to a face
struct FaceRecord
{
	FaceRecordVertex vertexArray[3];
};

// Holds a complete dataset of a parsed model
struct ModelData
{
	std::vector<VertexRecord> vertexVector;
	std::vector<FaceRecord> faceVector;
};

class OBJParser
{
private:
	void SplitString(std::string originalString, std::vector<std::string>* returnedString, std::string separator);
	void RemoveWhiteSpace(std::string originalString, std::vector<std::string>* returnedString);
	void ExtractVertexData(std::string faceData, FaceRecordVertex* vertexRecord);
	void RemoveLeadingCharacters(std::string* dataLine, int numberOfCharacters);
	void ParseFaceData(std::vector<std::string>* data, std::vector<FaceRecord>* returnFaceData);
	void ParseVertexData(std::vector<std::string>* data, std::vector<VertexRecord>* returnVertexData);

public:
	void ParseOBJ(std::vector<std::string>* rawData, ModelData* parsedData);
	void TestFunction();
};
