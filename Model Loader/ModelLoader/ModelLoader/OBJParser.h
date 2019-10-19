#pragma once

using namespace std;

struct ModelData;
struct VertexRecord;
struct FaceRecord;
struct FaceRecordVertex;

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
	void TestFunction();
};
