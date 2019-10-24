#include <iostream>
#include <vector>
#include <string>
#include "OBJParser.h"

using namespace std;

bool OBJParser::ParseOBJ(vector<string>* rawData, ModelData* parsedData)
{
	vector<FaceRecord> faceData;
	vector<VertexRecord> vertexData;

	vector<string> vertices;
	vector<string> faces;

	for (int i = 0; i < rawData->size(); i++)
	{
		if ((*rawData)[i].substr(0, 2).compare("f ") == 0)
		{
			faces.push_back((*rawData)[i]);
		}
		else if ((*rawData)[i].substr(0, 2).compare("v ") == 0)
		{
			vertices.push_back((*rawData)[i]);
		}
	}

	try
	{
		ParseFaceData(&faces, &faceData);
		ParseVertexData(&vertices, &vertexData);

		parsedData->faceVector = faceData;
		parsedData->vertexVector = vertexData;
	}
	catch (const exception e)
	{
		cout << "OBJ Parsing Failed." << endl;
		return false;
	}

	return true;
}

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

void OBJParser::ParseVertexData(vector<string>* data, vector<VertexRecord>* returnVertexData)
{
	for (int i = 0; i < data->size(); i++)
	{
		VertexRecord vertexRecord;
		vector<string> vertexData;
		string dataString;

		dataString = (*data)[i];
		RemoveLeadingCharacters(&dataString, 2);
		RemoveWhiteSpace(dataString, &vertexData);

		// All coordinate values stored as floats
		vertexRecord.x = stof(vertexData[0]);
		vertexRecord.y = stof(vertexData[1]);
		vertexRecord.z = stof(vertexData[2]);

		// Not all OBJ files contain W values, so set to 1 if they're not present
		if (vertexData.size() < 3)
		{
			vertexRecord.w = stof(vertexData[3]);
		}
		else
		{
			vertexRecord.w = 1;
		}

		returnVertexData->push_back(vertexRecord);
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
	(*vertexRecord).vertexIndex = stoi(returnedString[0]);
	(*vertexRecord).textureIndex = stoi(returnedString[1]);
	(*vertexRecord).normalIndex = stoi(returnedString[2]);
}

void OBJParser::RemoveLeadingCharacters(string* dataLine, int numberOfCharacters)
{
	// Remove the first numberOfCharacters characters from the string
	(*dataLine) = dataLine->substr(numberOfCharacters, dataLine->length() - numberOfCharacters);
}

void OBJParser::TestFunction()
{
	vector<string> faceData;
	vector<string> vectorData;
	vector<FaceRecord> returnFaceData;
	vector<VertexRecord> returnVectorData;

	string faceTestOne = "f 12/13/14 15/16/17 18/19/20";
	string faceTestTwo = "f 22/23/24 25/26/27 28/29/30";
	string faceTestThree = "f 32/33/34 35/36/37 38/39/40";

	string vectorTestOne = "v 0.01 0.02 0.03";
	string vectorTestTwo = "v 0.04 0.05 0.06";
	string vectorTestThree = "v 0.07 0.08 0.09";

	faceData.push_back(faceTestOne);
	faceData.push_back(faceTestTwo);
	faceData.push_back(faceTestThree);

	vectorData.push_back(vectorTestOne);
	vectorData.push_back(vectorTestTwo);
	vectorData.push_back(vectorTestThree);

	ParseFaceData(&faceData, &returnFaceData);
	ParseVertexData(&vectorData, &returnVectorData);

	cout << "Parsed Face Data: " << endl << endl;

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

	cout << endl << "Parsed Vector Data: " << endl << endl;

	for (int i = 0; i < returnVectorData.size(); i++)
	{
		cout << "X: " << returnVectorData[i].x << " Y: " << returnVectorData[i].y <<
			" Z: " << returnVectorData[i].z << " W: " << returnVectorData[i].w;

		cout << endl << endl;
	}
}