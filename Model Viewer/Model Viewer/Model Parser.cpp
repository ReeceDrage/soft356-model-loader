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

bool OBJParser::ParseOBJ(vector<string> rawData, Model *model)
{
	vector<vector<string>> data;
	int numberOfObjects = 0;

	int vertexOffset = 0;
	int textureOffset = 0;
	int normalOffset = 0;

	for (int i = 0; i < rawData.size(); i++)
	{
		// Split each string for easy access to the identifying prefix
		vector<string> splitString;
		StringSplit(rawData[i], &splitString, ' ', true);

		// If an object is found
		if (splitString[0] == "o")
		{
			// Ignore the initial comments
			if (numberOfObjects > 0)
			{
				ModelObject object;
				ParseObject(&data, &object, &vertexOffset, &textureOffset, &normalOffset);
				model->objects.push_back(object);
				data.clear();
			}

			numberOfObjects++;
		}
		else
		{
			data.push_back(splitString);
		}
	}

	// Ensure the final object is parsed and stored, or the full structure if no objects were detected
	ModelObject object;
	ParseObject(&data, &object, &vertexOffset, &textureOffset, &normalOffset);
	model->objects.push_back(object);
	data.clear();

	return true;
}

bool OBJParser::ParseObject(const vector<vector<string>>* data, ModelObject* object, int* vertexOffset, int* textureOffset, int* normalOffset)
{
	// Variables to store coordinate values
	vector<vec4> tempVectorCoordinates;
	vector<vec2> tempTextureCoordinates;
	vector<vec4> tempNormalCoordinates;

	// Variables to handle faces and materials
	vector<string> materialNames;
	vector<vector<string>> materialsAndFaces;
	vector<FaceRecord> faceRecords;
	vector<vector<FaceRecord>> vertexGroupFaces;

	bool parseSuccessful = true;
	bool materialPresent = false;

	for (int i = 0; i < data->size(); i++)
	{
		// Retrieve vertex, texture and normal data, then parse it into buffers for processing
		if ((*data)[i][0] == "v")
		{
			vec4 vertex;
			parseSuccessful = ParseVertexCoordinates((*data)[i], &vertex);
			tempVectorCoordinates.push_back(vertex);
		}
		else if ((*data)[i][0] == "vt")
		{
			vec2 texture;
			parseSuccessful = ParseTextureCoordinates((*data)[i], &texture);
			tempTextureCoordinates.push_back(texture);
		}
		else if ((*data)[i][0] == "vn")
		{
			vec4 normal;
			parseSuccessful = ParseNormalCoordinates((*data)[i], &normal);
			tempNormalCoordinates.push_back(normal);
		}

		// Place material and face data into a temporary storage for parsing
		else if ((*data)[i][0] == "usemtl")
		{
			materialPresent = true;
			materialsAndFaces.push_back((*data)[i]);
		}
		else if ((*data)[i][0] == "f")
		{
			materialsAndFaces.push_back((*data)[i]);
		}
	}

	// If material data is used, parse into groups, otherwise parse as a single group
	if (materialPresent)
	{
		int startingIndex = 0;

		// Store the ID name of the first material
		materialNames.push_back(materialsAndFaces[0][1]);

		for (int i = 1; i < materialsAndFaces.size(); i++)
		{
			if (materialsAndFaces[i][0] == "usemtl")
			{
				// Store the ID name of the material
				materialNames.push_back(materialsAndFaces[i][1]);

				// Parse between the last two usemtl lines identified
				for (int j = startingIndex + 1; j < i; j++)
				{
					// Retrieve the line, parse and add it to the vector
					FaceRecord face;
					parseSuccessful = ParseFaceCoordinates(materialsAndFaces[j], &face);
					faceRecords.push_back(face);
					face.vertexVector.clear();
				}

				// Add the vector to the vertexGroup vector
				startingIndex = i;
				vertexGroupFaces.push_back(faceRecords);
				faceRecords.clear();
			}

			// Ensure the last group is also parsed correctly
			else if (i == materialsAndFaces.size() - 1)
			{
				for (int j = startingIndex + 1; j <= i; j++)
				{
					// Retrieve the line, parse and add it to the vector
					FaceRecord face;
					parseSuccessful = ParseFaceCoordinates(materialsAndFaces[j], &face);
					faceRecords.push_back(face);
					face.vertexVector.clear();
				}

				// Add the vector to the vertexGroup vector
				vertexGroupFaces.push_back(faceRecords);
				faceRecords.clear();
			}
		}
	}
	else
	{
		FaceRecord face;

		for (int i = 0; i < materialsAndFaces.size(); i++)
		{
			// Retrieve the line, parse and add it to the vector
			FaceRecord face;
			parseSuccessful = ParseFaceCoordinates(materialsAndFaces[i], &face);
			faceRecords.push_back(face);
		}

		vertexGroupFaces.push_back(faceRecords);
	}

	// Process every face record to retrieve vertex, texture and normal indices
	for (int i = 0; i < vertexGroupFaces.size(); i ++)
	{
		VertexGroup vertexGroup;

		for (int j = 0; j < vertexGroupFaces[i].size(); j++)
		{
			FaceRecord face = vertexGroupFaces[i][j];

			// If quad data is found, split into a pair of triangles
			if (face.vertexVector.size() == 4)
			{
				// Triangle one
				vertexGroup.vertexCoordinates.push_back(tempVectorCoordinates[face.vertexVector[0].vertexIndex - (*vertexOffset) - 1]);
				vertexGroup.textureCoordinates.push_back(tempTextureCoordinates[face.vertexVector[0].textureIndex - (*textureOffset) - 1]);
				vertexGroup.normalCoordinates.push_back(tempNormalCoordinates[face.vertexVector[0].normalIndex - (*normalOffset) - 1]);

				vertexGroup.vertexCoordinates.push_back(tempVectorCoordinates[face.vertexVector[1].vertexIndex - (*vertexOffset) - 1]);
				vertexGroup.textureCoordinates.push_back(tempTextureCoordinates[face.vertexVector[1].textureIndex - (*textureOffset) - 1]);
				vertexGroup.normalCoordinates.push_back(tempNormalCoordinates[face.vertexVector[1].normalIndex - (*normalOffset) - 1]);

				vertexGroup.vertexCoordinates.push_back(tempVectorCoordinates[face.vertexVector[2].vertexIndex - (*vertexOffset) - 1]);
				vertexGroup.textureCoordinates.push_back(tempTextureCoordinates[face.vertexVector[2].textureIndex - (*textureOffset) - 1]);
				vertexGroup.normalCoordinates.push_back(tempNormalCoordinates[face.vertexVector[2].normalIndex - (*normalOffset) - 1]);

				// Triangle two
				vertexGroup.vertexCoordinates.push_back(tempVectorCoordinates[face.vertexVector[2].vertexIndex - (*vertexOffset) - 1]);
				vertexGroup.textureCoordinates.push_back(tempTextureCoordinates[face.vertexVector[2].textureIndex - (*textureOffset) - 1]);
				vertexGroup.normalCoordinates.push_back(tempNormalCoordinates[face.vertexVector[2].normalIndex - (*normalOffset) - 1]);

				vertexGroup.vertexCoordinates.push_back(tempVectorCoordinates[face.vertexVector[3].vertexIndex - (*vertexOffset) - 1]);
				vertexGroup.textureCoordinates.push_back(tempTextureCoordinates[face.vertexVector[3].textureIndex - (*textureOffset) - 1]);
				vertexGroup.normalCoordinates.push_back(tempNormalCoordinates[face.vertexVector[3].normalIndex - (*normalOffset) - 1]);

				vertexGroup.vertexCoordinates.push_back(tempVectorCoordinates[face.vertexVector[0].vertexIndex - (*vertexOffset) - 1]);
				vertexGroup.textureCoordinates.push_back(tempTextureCoordinates[face.vertexVector[0].textureIndex - (*textureOffset) - 1]);
				vertexGroup.normalCoordinates.push_back(tempNormalCoordinates[face.vertexVector[0].normalIndex - (*normalOffset) - 1]);
			}
			else
			{
				for (int k = 0; k < vertexGroupFaces[i][j].vertexVector.size(); k++)
				{
					if (face.vertexVector[k].vertexIndex > 0)
						vertexGroup.vertexCoordinates.push_back(tempVectorCoordinates[face.vertexVector[k].vertexIndex - (*vertexOffset) - 1]);

					if (face.vertexVector[k].textureIndex > 0)
						vertexGroup.textureCoordinates.push_back(tempTextureCoordinates[face.vertexVector[k].textureIndex - (*textureOffset) - 1]);

					if (face.vertexVector[k].normalIndex > 0)
						vertexGroup.normalCoordinates.push_back(tempNormalCoordinates[face.vertexVector[k].normalIndex - (*normalOffset) - 1]);
				}
			}
		}

		object->vertexGroups.push_back(vertexGroup);
	}

	(*vertexOffset) += tempVectorCoordinates.size();
	(*textureOffset) += tempTextureCoordinates.size();
	(*normalOffset) += tempNormalCoordinates.size();

	return parseSuccessful;
}

bool OBJParser::StringSplit(string data, vector<string>* splitData, char delimiter, bool removeWhiteSpace)
{
	// Create a string stream using the input string
	stringstream stream(data);
	string section;

	try
	{
		// Get each substring separated by the delimiter character and add it to the returnedString vector
		while (getline(stream, section, delimiter))
		{
			// Ignore empty entries if removeWhiteSpace is true
			if (section != "" || !removeWhiteSpace)
			{
				splitData->push_back(section);
			}
		}
	}
	catch (std::exception e)
	{
		AddToErrorString("String splitting process failed on string: " + data + ", splitting by: " + delimiter + ".");
		return false;
	}

	return true;
}

bool OBJParser::ParseVertexCoordinates(vector<string> data, vec4* vertex)
{
	try
	{
		// Convert string values to floating points
		vertex->x = stof(data[1]);
		vertex->y = stof(data[2]);
		vertex->z = stof(data[3]);

		// If a W value is present, convert it to a float. Otherwise, set it to 1
		if (data.size() > 4)
		{
			vertex->w = stof(data[4]);
		}
		else
		{
			vertex->w = 1;
		}
	}
	catch (std::exception e)
	{
		AddToErrorString("Vertex parsing failed. String -> floating point conversion was unsuccessful.");
		return false;
	}

	return true;
}

bool OBJParser::ParseFaceCoordinates(vector<string> data, FaceRecord* face)
{
	for (int i = 1; i < data.size(); i++)
	{
		FaceRecordVertex vertexRecord;
		vector<string> splitString;

		// Split the string by /, keeping empty entries intact
		bool splitSuccessful = StringSplit(data[i], &splitString, '/', false);

		try
		{
			// If string splitting fails, do not proceed
			if (!splitSuccessful)
			{
				throw 0;
			}

			if (splitString.size() == 3)
			{
				// Leave any empty components null
				if (splitString[0] != "")
					vertexRecord.vertexIndex = stoi(splitString[0]);

				if (splitString[1] != "")
					vertexRecord.textureIndex = stoi(splitString[1]);

				if (splitString[2] != "")
					vertexRecord.normalIndex = stoi(splitString[2]);
			}
			else if (splitString.size() == 2)
			{
				// Leave any empty components null
				if (splitString[0] != "")
					vertexRecord.vertexIndex = stoi(splitString[0]);

				if (splitString[1] != "")
					vertexRecord.textureIndex = stoi(splitString[1]);
			}
		}
		catch (std::exception e)
		{
			AddToErrorString("Face parsing failed. String -> integer conversion was unsuccessful.");
			return false;
		}

		face->vertexVector.push_back(vertexRecord);
		splitString.clear();
	}

	return true;
}

bool OBJParser::ParseTextureCoordinates(vector<string> data, vec2* texture)
{
	try
	{
		// Convert string values to floating points
		texture->x = stof(data[1]);
		texture->y = stof(data[2]);
	}
	catch (std::exception e)
	{
		AddToErrorString("Texture parsing failed. String -> floating point conversion was unsuccessful.");
		return false;
	}

	return true;
}

bool OBJParser::ParseNormalCoordinates(vector<string> data, vec4* normal)
{
	try
	{
		// Convert string values to floating points
		normal ->x = stof(data[1]);
		normal->y = stof(data[2]);
		normal->z = stof(data[3]);
		normal->w = 1;
	}
	catch (std::exception e)
	{
		AddToErrorString("Normal parsing failed. String -> floating point conversion was unsuccessful.");
		return false;
	}

	return true;
}

// Error message handling functions

void OBJParser::AddToErrorString(string errorMessage)
{
	OBJParser::errorMessage.push_back(errorMessage);
}

void OBJParser::GetErrorString(vector<string>* errorVector)
{
	for (int i = 0; i < OBJParser::errorMessage.size(); i++)
	{
		errorVector->push_back(errorMessage[i]);
	}

	// Clear the error message for the next parsing attempt
	errorMessage.clear();
}