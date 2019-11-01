// C/C++ include statements
#include <iostream>
#include <sstream>
#include <vector>

// Include statements for the glm maths library
#include "glm/glm.hpp"

// Include custom header files
#include "Model Parser.h"

using glm::vec2;
using glm::vec3;
using glm::vec4;

using std::vector;
using std::string;
using std::stringstream;

bool OBJParser::ParseOBJ(vector<string> rawData, Model model)
{
	return true;
}

bool OBJParser::StringSplit(string data, vector<string>* splitData, char delimiter)
{
	// Create a string stream using the input string
	stringstream stream(data);
	string section;

	try
	{
		// Get each substring separated by the delimiter character and add it to the returnedString vector
		while (getline(stream, section, delimiter))
		{
			// Ignore empty entries
			if (section != "")
			{
				splitData->push_back(section);
			}
		}
	}
	catch (std::exception e)
	{
		return false;
	}

	return true;
}

bool OBJParser::ParseVertexCoordinates(vector<string> data, vec4 vertex)
{
	return false;
}

bool OBJParser::ParseFaceCoordinates(vector<string> data, FaceRecord face)
{
	return false;
}

bool OBJParser::ParseTextureCoordinates(vector<string> data, vec2 texture)
{
	return false;
}

bool OBJParser::ParseNormalCoordinates(vector<string> data, vec4 normal)
{
	return false;
}