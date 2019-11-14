#pragma once

#include "ModelStructs.h"
#include <vector>

class ModelLoader
{
public:

	bool LoadModel(std::string fileName, Model* model, float* scaleValue);
	void GetErrorString(std::vector<std::string>* errorVector);

private:

	// Error handling
	std::vector<std::string> errorMessage;
	void AddToErrorString(std::string error);

	// Helper functions
	bool StringSplit(std::string data, std::vector<std::string>* splitData, char delimiter, bool removeWhiteSpace);
	bool LoadFile(vector<string>* data, string fileName);

	// Functions for parsing OBJ data
	bool ParseOBJ(std::vector<std::string> rawData, std::vector<MaterialData> materials, Model* model, float* scaleValue);
	bool ParseObject(const std::vector<std::vector<std::string>>* data, const std::vector<MaterialData> *materials, ModelObject* object, int* vertexOffset, int* textureOffset, int* normalOffset, float* scaleValue);
	bool ParseVertexCoordinates(std::vector<std::string> data, glm::vec4* vertex);
	bool ParseFaceCoordinates(std::vector<std::string> data, FaceRecord* face);
	bool ParseTextureCoordinates(std::vector<std::string> data, glm::vec2* texture);
	bool ParseNormalCoordinates(std::vector<std::string> data, glm::vec4* normal);

	// Functions for parsing MTL data
	std::string RetrieveMaterialLibraryName(vector<string> rawData);
	bool ParseMaterial(std::vector<std::string>* fileData, std::vector<MaterialData>* materialData, std::string fileDirectory);
};