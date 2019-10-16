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

public:
	void TestFunction();
};
