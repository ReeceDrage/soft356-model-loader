// Standard C/C++ include statements
#include <iostream>
#include <vector>
#include <string>
#include <sstream>

// Include statements for the glm maths library
#include "glm/glm.hpp"

// Include statements for external header files
#include "OBJParser.h"

using namespace std;
using namespace glm;

void OBJParser::RemoveLeadingCharacters(string* data, int numberOfCharacters)
{
	// Remove leading characters and return a string containing data
	(*data) = data->substr(numberOfCharacters, data->length());
}

int OBJParser::CalculateNumberOfLeadingCharacters(string data)
{
	// Create a stream from 2 characters in to the end of the string (Excluding "v ", "vn" etc.)
	stringstream stream(data.substr(2, data.length()));
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