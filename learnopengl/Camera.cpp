#include "SceneNode.h"
#include "Camera.h"
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

Camera::Camera(const glm::mat4 &projectionMatrix) :
	mFinalMatrix(1.0f),
	mProjectionMatrix(projectionMatrix),
	mRotationMatrix(1.0f),
	mTranslationMatrix(1.0f)
{
	ComputeFinalMatrix();
}

void Camera::Draw(unsigned int shaderId, const SceneNode &sceneRoot) const
{
	uint32_t viewMatrixUniformLocation = glGetUniformLocation(shaderId, "viewMatrix");
	uint32_t projectionMatrixUniformLocation = glGetUniformLocation(shaderId, "projectionMatrix");
	glUniformMatrix4fv(viewMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(mFinalMatrix));
	glUniformMatrix4fv(projectionMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(mProjectionMatrix));

	sceneRoot.Draw(shaderId, glm::mat4(1.f));
}

void Camera::Translate(const glm::vec3 & translationVector)
{
	mTranslationMatrix = glm::translate(mTranslationMatrix, translationVector);
	ComputeFinalMatrix();
}

void Camera::Rotate(float angle, const glm::vec3& rotationVector)
{
	mRotationMatrix = glm::rotate(mRotationMatrix, angle, rotationVector);
	ComputeFinalMatrix();
}

void Camera::HandleInput(GLFWwindow* window, float deltaTime)
{
	assert(window != nullptr);
	if (window == nullptr)
		return;

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		MoveLeft(deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		MoveRight(deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		MoveForward(deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		MoveBack(deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		MoveUp(deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		MoveDown(deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		RotateUp(deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		RotateDown(deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		RotateLeft(deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		RotateRight(deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
	{
		Reset();
	}
}

inline void Camera::ComputeFinalMatrix()
{
	mFinalMatrix = mRotationMatrix * mTranslationMatrix;
}

inline void Camera::MoveForward(float deltaTime)
{
	Translate(glm::vec4(0.f, 0.f, deltaTime, 1.0f) * mRotationMatrix);
}

inline void Camera::MoveBack(float deltaTime)
{
	Translate(glm::vec4(0.f, 0.f, -deltaTime, 1.0f) * mRotationMatrix);
}

inline void Camera::MoveRight(float deltaTime)
{
	Translate(glm::vec4(-deltaTime, 0.f, 0.f, 1.0f) * mRotationMatrix);
}

inline void Camera::MoveLeft(float deltaTime)
{
	Translate(glm::vec4(deltaTime, 0.f, 0.f, 1.0f) * mRotationMatrix);
}

inline void Camera::MoveUp(float deltaTime)
{
	Translate(glm::vec4(0.f, -deltaTime, 0.f, 1.0f));
}

inline void Camera::MoveDown(float deltaTime)
{
	Translate(glm::vec4(0.f, deltaTime, 0.f, 1.0f));
}

inline void Camera::RotateLeft(float deltaTime)
{
	Rotate(RotationPerSecond * -deltaTime, glm::vec3(0.f, 1.0f, 0.0f));
}

inline void Camera::RotateRight(float deltaTime)
{
	Rotate(RotationPerSecond * deltaTime, glm::vec3(0.f, 1.0f, 0.0f));
}

inline void Camera::RotateUp(float deltaTime)
{
	Rotate(RotationPerSecond * -deltaTime, glm::vec3(1.f, 0.0f, 0.0f));
}

inline void Camera::RotateDown(float deltaTime)
{
	Rotate(RotationPerSecond * deltaTime, glm::vec3(1.f, 0.0f, 0.0f));
}

inline void Camera::Reset()
{
	mTranslationMatrix = glm::mat4(1.0f);
	mRotationMatrix = glm::mat4(1.0f);
	ComputeFinalMatrix();
}