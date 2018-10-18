#include "Camera.h"
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>


Camera::Camera(const glm::mat4 &projectionMatrix, const glm::mat4 &transformationMatrix) :
	mFinalMatrix(1.0f),
	mProjectionMatrix(projectionMatrix),
	mTransformationMatrix(transformationMatrix)
{
	ComputeFinalMatrix();
}

void Camera::Draw(unsigned int shaderId, const SceneNode &sceneRoot) const
{
	uint32_t viewMatrixUniformLocation = glGetUniformLocation(shaderId, "viewMatrix");
	uint32_t projectionMatrixUniformLocation = glGetUniformLocation(shaderId, "projectionMatrix");
	glUniformMatrix4fv(viewMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(mTransformationMatrix));
	glUniformMatrix4fv(projectionMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(mProjectionMatrix));

	sceneRoot.Draw(shaderId, glm::mat4(1.f));
}

void Camera::SetTransformationMatrix(const glm::mat4 &transformationMatrix)
{
	mTransformationMatrix = transformationMatrix;
	ComputeFinalMatrix();
}

void Camera::Translate(const glm::vec3 & translationVector)
{
	mTransformationMatrix = glm::translate(mTransformationMatrix, translationVector);
	ComputeFinalMatrix();
}

void Camera::Rotate(float angle, const glm::vec3& rotationVector)
{
	mTransformationMatrix = glm::rotate(mTransformationMatrix, angle, rotationVector);
	ComputeFinalMatrix();
}

inline void Camera::ComputeFinalMatrix()
{
	mFinalMatrix = mProjectionMatrix * mTransformationMatrix;
}
