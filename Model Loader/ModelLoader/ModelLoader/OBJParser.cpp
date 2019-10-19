#include <iostream>
#include <vector>
#include "OBJParser.h"

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

void OBJParser::SplitString(string originalString, vector<string>* returnedString, string separator)
{
	// Create a buffer and set indices for string indexing to 0
	string buffer;

	size_t startingIndex = 0;
	size_t foundIndex = 0;

	// While foundIndex does not equal the termination value, repeatedly find the next instance 
	// of the separator string and return the index
	while ((foundIndex = originalString.find(separator, foundIndex + 1)) != string::npos)
	{
		// Get the substring from the start of the substring to the found separator and store
		buffer = originalString.substr(startingIndex, foundIndex - startingIndex);
		returnedString->push_back(buffer);

		// Start from the end of the previous substring on the next iteration
		startingIndex = foundIndex + 1;
	}

	// Get the final substring from the end of the previous substring to the end of the string and store
	buffer = originalString.substr(startingIndex, originalString.length() - startingIndex);
	returnedString->push_back(buffer);
}

void OBJParser::RemoveWhiteSpace(string originalString, vector<string>* returnedString)
{
	string seperator = " ";
	SplitString(originalString, returnedString, seperator);
}

void OBJParser::TestFunction()
{
	string testString = "This is my testing string";
	vector<string> finalString;

	RemoveWhiteSpace(testString, &finalString);

	cout << "Test string: " << testString << endl;
	cout << "Final string : " << endl << endl;

	for (int i = 0; i < finalString.size(); i++)
	{
		cout << finalString[i] << endl;
	}
}