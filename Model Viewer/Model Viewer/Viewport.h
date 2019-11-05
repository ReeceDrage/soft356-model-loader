#pragma once

// Include statements for the glm maths library
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

class Viewport
{
public:
	void Initialize(float fieldOfView, float windowWidth, float windowHeight);

	glm::mat4 modelMatrix;
	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;

	// Used to modify the MVP
	void ScaleModel(float x, float y, float z);
	void RotateModel(float angle, glm::vec3 upVector);
	void TranslateModel(float x, float y, float z);
	void TranslateView(float x, float y, float z);
	void ViewLookAt(glm::vec3 cameraPosition, glm::vec3 cameraTarget, glm::vec3 upVector);
	void SetProjectionFieldOfView(float fieldOfView, float windowWidth, float windowHeight);

	// Getter method
	glm::mat4 GetMVPMatrix();

private:

};