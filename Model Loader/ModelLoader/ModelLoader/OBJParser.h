#pragma once

using namespace std;

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
	float vertexIndex;
	float textureIndex;
	float normalIndex;
};

// Holds records of each vertex linked to a face
struct FaceRecord
{
	FaceRecordVertex vertexArray[3];
};

// Holds a complete dataset of a parsed model
struct ModelData
{
	vector<VertexRecord> vertexVector;
	vector<FaceRecord> faceVector;
};

class OBJParser
{
private:
	void SplitString(string originalString, vector<string>* returnedString, string separator);
	void RemoveWhiteSpace(string originalString, vector<string>* returnedString);
	void ExtractVertexData(string faceData, FaceRecordVertex* vertexRecord);
	void RemoveLeadingCharacters(string* dataLine, int numberOfCharacters);
	void ParseFaceData(vector<string>* data, vector<FaceRecord>* returnFaceData);
	void ParseVertexData(vector<string>* data, vector<VertexRecord>* returnVertexData);

public:
	void ParseOBJ(vector<string>* rawData, ModelData* parsedData);
	void TestFunction();
};
