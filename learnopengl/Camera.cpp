#include "Camera.h"



Camera::Camera(const glm::mat4 &projectionMatrix) :
	mProjectionMatrix(1.0f)
{
	mProjectionMatrix = projectionMatrix;
}

void Camera::Draw(unsigned int shaderId, const SceneNode &sceneRoot) const
{
	sceneRoot.Draw(shaderId, mProjectionMatrix);
}

