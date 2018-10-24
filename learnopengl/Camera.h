#pragma once

#include <GLFW\glfw3.h>
#include <glm\glm.hpp>

class SceneNode;

class Camera
{
public:
	Camera(const glm::mat4 &projectionMatrix);
	~Camera() = default;

	void Draw(unsigned int shaderId, const SceneNode &sceneRoot) const;

	void Translate(const glm::vec3 &translationVector);
	void Rotate(float angle, const glm::vec3& rotationVector);
	void HandleInput(GLFWwindow *window, float deltaTime);
private:
	const float RotationPerSecond = glm::radians(30.f);

	glm::mat4 mFinalMatrix;

	glm::mat4 mProjectionMatrix;
	glm::mat4 mRotationMatrix;
	glm::mat4 mTranslationMatrix;

	inline void ComputeFinalMatrix();

	inline void MoveForward(float deltaTime);
	inline void MoveBack(float deltaTime);
	inline void MoveRight(float deltaTime);
	inline void MoveLeft(float deltaTime);
	inline void MoveUp(float deltaTime);
	inline void MoveDown(float deltaTime);
	inline void RotateLeft(float deltaTime);
	inline void RotateRight(float deltaTime);
	inline void RotateUp(float deltaTime);
	inline void RotateDown(float deltaTime);
	inline void Reset();
};

