#include "Camera.h"
#include <glm\gtc\matrix_transform.hpp>


Camera::Camera(const glm::mat4 &projectionMatrix, const glm::mat4 &transformationMatrix) :
	mFinalMatrix(1.0f),
	mProjectionMatrix(projectionMatrix),
	mTransformationMatrix(transformationMatrix)
{
	ComputeFinalMatrix();
}

void Camera::Draw(unsigned int shaderId, const SceneNode &sceneRoot) const
{
	sceneRoot.Draw(shaderId, mFinalMatrix);
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
