#pragma once

#include <glm\glm.hpp>

#include "SceneNode.h"

class Camera
{
public:
	Camera(const glm::mat4 &projectionMatrix, const glm::mat4 &transformationMatrix);
	~Camera() = default;

	void Draw(unsigned int shaderId, const SceneNode &sceneRoot) const;
	void SetTransformationMatrix(const glm::mat4 &transformationMatrix);

	void Translate(const glm::vec3 &translationVector);
	void Rotate(float angle, const glm::vec3& rotationVector);
private:
	glm::mat4 mFinalMatrix;

	glm::mat4 mProjectionMatrix;
	glm::mat4 mTransformationMatrix;

	inline void ComputeFinalMatrix();
};

