#include <iostream>
#include <vector>
#include <string>
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
	FaceRecordVertex vertexArray[3];
};

// Holds a complete dataset of a parsed model
struct ModelData
{
	vector<VertexRecord> vertexVector;
	vector<FaceRecord> faceVector;
};

void OBJParser::ParseFaceData(vector<string>* data, vector<FaceRecord>* returnFaceData)
{
	// For every string in the data, iterate, remove unneeded characters and add extracted data to an array
	for (int i = 0; i < data->size(); i++)
	{
		FaceRecord triangleRecord;
		vector<string> vertexData;
		string dataString;

		dataString = (*data)[i];
		RemoveLeadingCharacters(&dataString, 2);
		RemoveWhiteSpace(dataString, &vertexData);

		for (int j = 0; j < 3; j++)
		{
			FaceRecordVertex vertexRecord;

			ExtractVertexData(vertexData[j], &vertexRecord);
			triangleRecord.vertexArray[j] = vertexRecord;
		}

		returnFaceData->push_back(triangleRecord);
	}
}

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

void OBJParser::ExtractVertexData(string faceData, FaceRecordVertex* vertexRecord)
{
	string seperator = "/";
	vector<string> returnedString;

	// Convert returned strings into float values
	SplitString(faceData, &returnedString, seperator);
	(*vertexRecord).vertexIndex = stof(returnedString[0]);
	(*vertexRecord).textureIndex = stof(returnedString[1]);
	(*vertexRecord).normalIndex = stof(returnedString[2]);
}

void OBJParser::RemoveLeadingCharacters(string* dataLine, int numberOfCharacters)
{
	// Remove the first numberOfCharacters characters from the string
	(*dataLine) = dataLine->substr(numberOfCharacters, dataLine->length() - numberOfCharacters);
}

void OBJParser::TestFunction()
{
	vector<string> data;
	vector<FaceRecord> returnFaceData;

	string testStringOne = "f 12/13/14 15/16/17 18/19/20";
	string testStringTwo = "f 22/23/24 25/26/27 28/29/30";
	string testStringThree = "f 32/33/34 35/36/37 38/39/40";

	data.push_back(testStringOne);
	data.push_back(testStringTwo);
	data.push_back(testStringThree);

	ParseFaceData(&data, &returnFaceData);

	cout << "Parsed Data: " << endl << endl;

	for (int i = 0; i < returnFaceData.size(); i++)
	{
		for (int j = 0; j < 3; j++)
		{
			cout << "Vertex Index: " << returnFaceData[i].vertexArray[j].vertexIndex << endl;
			cout << "Texture Index: " << returnFaceData[i].vertexArray[j].textureIndex << endl;
			cout << "Normal Index: " << returnFaceData[i].vertexArray[j].normalIndex << endl;
			cout << endl;
		}

		cout << endl;
	}
}