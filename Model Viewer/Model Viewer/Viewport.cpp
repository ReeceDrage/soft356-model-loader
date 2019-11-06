#include "Viewport.h"

void Viewport::Initialize(float fieldOfView, float windowWidth, float windowHeight)
{
	// Generate initial model, view and projection matrices with default values
	modelMatrix = glm::mat4(1.0f);
	viewMatrix = glm::lookAt(glm::vec3(4, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	projectionMatrix = glm::perspective(glm::radians(fieldOfView), windowWidth / windowHeight, 0.1f, 1000.0f);
}

void Viewport::ScaleModel(float x, float y, float z) 
{
	modelMatrix = glm::scale(modelMatrix, glm::vec3(x, y, z));
}

void Viewport::RotateModel(float angle, glm::vec3 upVector)
{
	modelMatrix = glm::rotate(modelMatrix, glm::radians(angle), upVector);
}

void Viewport::TranslateModel(float x, float y, float z)
{
	modelMatrix = glm::translate(modelMatrix, glm::vec3(x, y, z));
}

void Viewport::TranslateView(float x, float y, float z)
{
	viewMatrix = glm::translate(viewMatrix, glm::vec3(x, y, z));
}

void Viewport::ViewLookAt(glm::vec3 cameraPosition, glm::vec3 cameraTarget, glm::vec3 upVector)
{
	viewMatrix = glm::lookAt(cameraPosition, cameraTarget, upVector);
}

void Viewport::SetProjectionFieldOfView(float fieldOfView, float windowWidth, float windowHeight)
{
	projectionMatrix = glm::perspective(glm::radians(fieldOfView), windowWidth / windowHeight, 0.1f, 500.0f);
}

glm::mat4 Viewport::GetMVPMatrix()
{
	return projectionMatrix * viewMatrix * modelMatrix;
}