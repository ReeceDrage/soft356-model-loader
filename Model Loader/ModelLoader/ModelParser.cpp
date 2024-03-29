// Standard C/C++ include statements
#include <iostream>
#include <vector>
#include <string>
#include <sstream>

// Include statements for the glm maths library
#include "glm/glm.hpp"

// Include statements for created header files
#include "ModelParser.h"

using std::string;
using std::vector;
using glm::vec2;
using glm::vec4;

void ModelParser::SetStrategy(string input)
{
	if (input == "OBJ")
	{
		ModelParser::strategy = new OBJParser();
	}
	else
	{
		ModelParser::strategy = NULL;
	}
}

bool ModelParser::ParseModel(vector<string> rawData, vector<vec4>* vertices, vector<vec2>* textures, vector<vec4>* normals)
{
	try
	{
		ModelParser::strategy->Parse(rawData, vertices, textures, normals);
	}
	catch (std::exception e)
	{
		return false;
	}
	
	return true;
}

bool OBJParser::Parse(std::vector<std::string> rawData, std::vector<glm::vec4>* vertices, std::vector<glm::vec2>* textures, std::vector<glm::vec4>* normals)
{
	bool parsingSuccessful = true;
	vector<FaceRecord> tempFaces;
	vector<vec4> tempVertices;

	for (int i = 0; i < rawData.size(); i++)
	{
		// If parsing fails at any point, stop parsing
		if (parsingSuccessful)
		{
			if (rawData[i].substr(0, 2).compare("f ") == 0)
			{
				FaceRecord face;
				parsingSuccessful = ParseFaceData(rawData[i], &face);
				tempFaces.push_back(face);
			}
			else if (rawData[i].substr(0, 2).compare("v ") == 0)
			{
				vec4 vertex;
				parsingSuccessful = ParseVertex(rawData[i], &vertex);
				tempVertices.push_back(vertex);
			}
		}
		else
		{
			return false;
		}
	}

	// Calculate exact vertices need from face data
	for (int i = 0; i < tempFaces.size(); i++)
	{
		for (int j = 0; j < 3; j++)
		{
			int index = tempFaces[i].vertexArray[j].vertexIndex - 1;
			vertices->push_back(tempVertices[index]);
		}
	}

	std::cout << "Vertex calculation successful." << std::endl << std::endl;

	return true;
}

bool OBJParser::ParseVertex(string data, vec4* vertex)
{
	vector<string> splitString;

	// Remove the first x characters and then split the string by " "
	int charactersToRemove = CalculateNumberOfLeadingCharacters(data);
	RemoveLeadingCharacters(&data, charactersToRemove);
	StringSplit(data, &splitString, ' ');

	try
	{
		// All coordinate values stored as floats
		vertex->x = stof(splitString[0]);
		vertex->y = stof(splitString[1]);
		vertex->z = stof(splitString[2]);

		// Not all OBJ files contain W values, so set to 1 if they're not present
		if (splitString.size() < 3)
		{
			vertex->w = stof(splitString[3]);
		}
		else
		{
			vertex->w = 1;
		}
	}
	catch (std::exception e)
	{
		return false;
	}

	return true;
}

bool OBJParser::ParseFaceData(string data, FaceRecord* face)
{
	vector<string> splitString;
	FaceRecordVertex faceRecordVertex;

	// Remove the first x characters and then split the string by " "
	int charactersToRemove = CalculateNumberOfLeadingCharacters(data);
	RemoveLeadingCharacters(&data, charactersToRemove);
	StringSplit(data, &splitString, ' ');

	try
	{
		// For every record in splitString, split by '/' and convert into integers
		for (int i = 0; i < splitString.size(); i++)
		{
			vector<string> splitFace;
			StringSplit(splitString[i], &splitFace, '/');

			faceRecordVertex.vertexIndex = stoi(splitFace[0]);
			faceRecordVertex.textureIndex = stoi(splitFace[1]);
			faceRecordVertex.normalIndex = stoi(splitFace[2]);

			face->vertexArray[i] = faceRecordVertex;
		}
	}
	catch (std::exception e)
	{
		return false;
	}

	return true;
}

void OBJParser::StringSplit(string data, vector<string>* returnedString, char delimiter)
{
	// Create a string stream using the input string
	std::stringstream stream(data);
	string section;

	// Get each substring separated by the delimiter character and add it to the returnedString vector
	while (getline(stream, section, delimiter))
	{
		returnedString->push_back(section);
	}
}

void OBJParser::RemoveLeadingCharacters(string* data, int numberOfCharacters)
{
	// Remove leading characters and return a string containing data
	(*data) = data->substr(numberOfCharacters, data->length());
}

int OBJParser::CalculateNumberOfLeadingCharacters(string data)
{
	// Create a stream from 2 characters in to the end of the string (Excluding "v ", "vn" etc.)
	std::stringstream stream(data.substr(2, data.length()));
	string section;
	int counter = 0;

	// Count the blank spaces
	while (stream.get() == ' ')
	{
		counter++;
	}

	// Return the number of blank spaces plus the leading 2 characters
	return counter + 2;
}