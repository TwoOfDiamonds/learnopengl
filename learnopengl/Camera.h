#pragma once

#include <glm\glm.hpp>

#include "SceneNode.h"

class Camera
{
public:
	Camera(const glm::mat4 &projectionMatrix);
	~Camera() = default;

	void Draw(unsigned int shaderId, const SceneNode &sceneRoot) const;
private:
	glm::mat4 mProjectionMatrix;
};

