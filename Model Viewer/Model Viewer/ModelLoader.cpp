// C/C++ include statements
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <regex>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Include statements for the glm maths library
#include "glm/glm.hpp"

// Include custom header files
#include "ModelLoader.h"

using glm::vec2;
using glm::vec3;
using glm::vec4;

using std::vector;
using std::string;
using std::stringstream;

bool ModelLoader::LoadFile(vector<string>* data, string fileName)
{
	// if file extension is not OBJ or MTL, the file is not supported, so do not load or parse.
	string fileExtension = fileName.substr(fileName.size() - 3, fileName.size());
	if (fileExtension == "obj" || fileExtension == "OBJ" || fileExtension == "mtl" || fileExtension == "MTL")
	{
		try
		{
			// Create input stream
			string line;
			std::ifstream myFile(fileName);

			// If the file is open, retrieve lines from the input stream
			if (myFile.is_open())
			{
				while (getline(myFile, line))
				{
					data->push_back(line);
				}

				myFile.close();
			}
			else
			{
				throw std::exception();
			}
		}
		catch (std::exception e)
		{
			AddToErrorString("File loading of file: " + fileName + " failed.");
			return false;
		}

		return true;
	}
	else
	{
		AddToErrorString("Unsupported file type.");
		return false;
	}
}

bool ModelLoader::LoadModel(std::string fileName, Model* model, float* scaleValue)
{
	std::vector<std::string> mtlFileData;
	std::vector<std::string> objFileData;
	std::vector<MaterialData> materialData;

	std::vector<std::string> splitFilePath;
	std::string fileDirectory = "";

	bool fileLoadSuccessful = false;

	// Split the file name to retrieve the directory
	StringSplit(fileName, &splitFilePath, '/', true);
	for (int i = 0; i < splitFilePath.size() - 1; i++)
	{
		fileDirectory += splitFilePath[i] + "/";
	}

	// Load obj file into string vectors
	fileLoadSuccessful = LoadFile(&objFileData, fileName);

	string materialFileName = RetrieveMaterialLibraryName(objFileData);

	if (materialFileName != "")
	{
		fileLoadSuccessful = LoadFile(&mtlFileData, fileDirectory + materialFileName);
	}

	if (!fileLoadSuccessful)
	{
		return false;
	}

	if (mtlFileData.size() > 0)
	{
		ParseMaterial(&mtlFileData, &materialData, fileDirectory);
	}

	if (objFileData.size() > 0)
	{
		ParseOBJ(objFileData, materialData, model, scaleValue);
	}

	(*scaleValue) = 1 / (*scaleValue);

	return true;
}

void RegexIterator(std::string stringToIterateThrough, std::regex regularExpression, vector<string> *returnedValues)
{
	auto tokensBegin = std::sregex_iterator(stringToIterateThrough.begin(), stringToIterateThrough.end(), regularExpression);
	auto tokensEnd = std::sregex_iterator();

	for (std::sregex_iterator i = tokensBegin; i != tokensEnd; i++)
	{
		std::smatch match = *i;
		returnedValues->push_back(match[0].str());
	}
}

bool ModelLoader::ParseMaterial(std::vector<std::string>* fileData, std::vector<MaterialData>* materialData, std::string fileDirectory)
{
	MaterialData *data = new MaterialData();
	data->materialName = "";
	bool isFirstElement = true;

	// Regex expressions to identify full lines
	std::regex materialTitle("(newmtl)(.)*");
	std::regex specularExponent("(Ns)(.)*");
	std::regex ambientColour("(Ka)(.)*");
	std::regex diffusionColour("(Kd)(.)*");
	std::regex specularColour("(Ks)(.)*");
	std::regex emissionColour("(Ke)(.)*");
	std::regex illuminationType("(illum)(.)*");
	std::regex diffuseTextureMap("(map_Kd)(.)*");

	// Regex expressions to retrieve values
	std::regex floatValue("(\\d.\\d+)");
	std::regex integerValue("(\\d+)");

	for (int i = 0; i < fileData->size(); i++)
	{
		string retrievedString = (*fileData)[i];

		// Get material title
		if (std::regex_match(retrievedString, materialTitle))
		{
			// Ensure an empty MaterialData struct is not pushed to materialData
			if (isFirstElement)
			{
				// Set the material name as the substring after "newmtl "
				data->materialName = retrievedString.substr(7, retrievedString.size());

				isFirstElement = false;
			}
			else
			{
				materialData->push_back(*data);

				// Delete old MaterialData instance and create a new one
				delete(data);
				data = new MaterialData();

				// Set the material name as the substring after "newmtl "
				data->materialName = retrievedString.substr(7, retrievedString.size());
			}
		}

		// Get diffuse texture map
		else if (std::regex_match(retrievedString, diffuseTextureMap))
		{
			stbi_set_flip_vertically_on_load(true);
			std::string textureFilePath = fileDirectory + retrievedString.substr(7, retrievedString.size());

			// Load the texture data into a temporary buffer
			unsigned char* textureBuffer;
			textureBuffer = stbi_load(textureFilePath.c_str(), &(data->ambientTextureMap.width), &(data->ambientTextureMap.height), &(data->ambientTextureMap.channelCount), 0);
			stbi_image_free(data->ambientTextureMap.texture);

			// Allocate memory and copy the data to it
			(*data).ambientTextureMap.texture = (unsigned char*)malloc(data->ambientTextureMap.width * data->ambientTextureMap.height * data->ambientTextureMap.channelCount * sizeof(unsigned char));
			memcpy((*data).ambientTextureMap.texture, textureBuffer, data->ambientTextureMap.width * data->ambientTextureMap.height * data->ambientTextureMap.channelCount * sizeof(unsigned char));
		}

		// Get specular exponent
		else if (std::regex_match(retrievedString, specularExponent))
		{
			vector<string> stringVector;

			RegexIterator(retrievedString, floatValue, &stringVector);

			data->specularExponent = stof(stringVector[0]);
		}

		// Get ambient colour
		else if (std::regex_match(retrievedString, ambientColour))
		{
			vector<string> stringVector;

			RegexIterator(retrievedString, floatValue, &stringVector);

			// if a values are present, parse them in. Else, set the a value to 1.0
			if (stringVector.size() > 3)
				data->ambientColour = glm::vec4(stof(stringVector[0]), stof(stringVector[1]), stof(stringVector[2]), stof(stringVector[3]));
			else
				data->ambientColour = glm::vec4(stof(stringVector[0]), stof(stringVector[1]), stof(stringVector[2]), 1.0f);
		}

		// Get diffusion colour
		else if (std::regex_match(retrievedString, diffusionColour))
		{
			vector<string> stringVector;

			RegexIterator(retrievedString, floatValue, &stringVector);

			// if a values are present, parse them in. Else, set the a value to 1.0
			if (stringVector.size() > 3)
				data->diffuseColour = glm::vec4(stof(stringVector[0]), stof(stringVector[1]), stof(stringVector[2]), stof(stringVector[3]));
			else
				data->diffuseColour = glm::vec4(stof(stringVector[0]), stof(stringVector[1]), stof(stringVector[2]), 1.0f);
		}

		// Get specular colour
		else if (std::regex_match(retrievedString, specularColour))
		{
			vector<string> stringVector;

			RegexIterator(retrievedString, floatValue, &stringVector);

			// if a values are present, parse them in. Else, set the a value to 1.0
			if (stringVector.size() > 3)
				data->specularColour = glm::vec4(stof(stringVector[0]), stof(stringVector[1]), stof(stringVector[2]), stof(stringVector[3]));
			else
				data->specularColour = glm::vec4(stof(stringVector[0]), stof(stringVector[1]), stof(stringVector[2]), 1.0f);
		}

		// get emission colour
		else if (std::regex_match(retrievedString, emissionColour))
		{
			vector<string> stringVector;

			RegexIterator(retrievedString, floatValue, &stringVector);

			// if a values are present, parse them in. Else, set the a value to 1.0
			if (stringVector.size() > 3)
				data->emissionColour = glm::vec4(stof(stringVector[0]), stof(stringVector[1]), stof(stringVector[2]), stof(stringVector[3]));
			else
				data->emissionColour = glm::vec4(stof(stringVector[0]), stof(stringVector[1]), stof(stringVector[2]), 1.0f);
		}

		// Get illumination type
		else if (std::regex_match(retrievedString, illuminationType))
		{
			vector<string> stringVector;

			RegexIterator(retrievedString, integerValue, &stringVector);

			data->illuminationModel = stoi(stringVector[0]);
		}
	}

	// Push the last material, then delete the memory from the heap
	materialData->push_back(*data);
	delete(data);

	return true;
}

string ModelLoader::RetrieveMaterialLibraryName(vector<string> rawData)
{
	for (int i = 0; i < rawData.size(); i++)
	{
		// Split each string for easy access to the identifying prefix
		vector<string> splitString;
		StringSplit(rawData[i], &splitString, ' ', true);

		// If the line starts with mtllib, return the filename
		if (splitString[0] == "mtllib")
		{
			return splitString[1];
		}
	}

	// Return an empty screen
	return "";
}

bool ModelLoader::StringSplit(std::string data, std::vector<std::string>* splitData, char delimiter, bool removeWhiteSpace)
{
	// Create a string stream using the input string
	std::stringstream stream(data);
	std::string section;

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
		return false;
	}

	return true;
}

bool ModelLoader::ParseOBJ(vector<string> rawData, vector<MaterialData> materials, Model *model, float * scaleValue)
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
				ParseObject(&data, &materials, &object, &vertexOffset, &textureOffset, &normalOffset, scaleValue);
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
	ParseObject(&data, &materials, &object, &vertexOffset, &textureOffset, &normalOffset, scaleValue);
	model->objects.push_back(object);
	data.clear();

	return true;
}

bool ModelLoader::ParseObject(const vector<vector<string>>* data, const vector<MaterialData> *materials, ModelObject* object, int* vertexOffset, int* textureOffset, int* normalOffset, float* scaleValue)
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

			if (abs(vertex.x) > (*scaleValue))
			{
				(*scaleValue) = abs(vertex.x);
			}

			if (abs(vertex.y) > (*scaleValue))
			{
				(*scaleValue) = abs(vertex.y);
			}

			if (abs(vertex.y) > (*scaleValue))
			{
				(*scaleValue) = abs(vertex.y);
			}

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

		for (int j = 0; j < materials->size(); j++)
		{
			if ((*materials)[j].materialName == materialNames[i])
			{
				vertexGroup.material = (*materials)[j];
			}
		}

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

bool ModelLoader::ParseVertexCoordinates(vector<string> data, vec4* vertex)
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

bool ModelLoader::ParseFaceCoordinates(vector<string> data, FaceRecord* face)
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
				throw std::exception();
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

bool ModelLoader::ParseTextureCoordinates(vector<string> data, vec2* texture)
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

bool ModelLoader::ParseNormalCoordinates(vector<string> data, vec4* normal)
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

void ModelLoader::AddToErrorString(string errorMessage)
{
	ModelLoader::errorMessage.push_back(errorMessage);
}

void ModelLoader::GetErrorString(vector<string>* errorVector)
{
	for (int i = 0; i < ModelLoader::errorMessage.size(); i++)
	{
		errorVector->push_back(errorMessage[i]);
	}

	// Clear the error message for the next parsing attempt
	errorMessage.clear();
}