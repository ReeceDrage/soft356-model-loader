#include <iostream>
#include <vector>

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
	vector<FaceRecordVertex> vertexVector;
};

// Holds a complete dataset of a parsed model
struct ModelData
{
	vector<VertexRecord> vertexVector;
	vector<FaceRecord> faceVector;
};

void TestFunction()
{
	cout << "Test Function Run" << endl;
}