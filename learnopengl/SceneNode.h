#pragma once

#include <glad\glad.h>
#include <list>
#include <memory>
#include <glm\glm.hpp>

#include "ObjFileParser.h"

class Geometry
{
public:
	std::vector<float> allVertices;
	std::string groupName;
	~Geometry() = default;
};

class SceneNode
{
public:
	SceneNode(const SceneNode &otherSceneNode);
	SceneNode(SceneNode &&otherSceneNode);
	SceneNode(std::unique_ptr<Geometry> geometry = nullptr, std::shared_ptr<TGAFile> textureData = nullptr);
	~SceneNode() = default;

	using SceneNodePtr = std::shared_ptr<SceneNode>;

	void AttachChild(SceneNodePtr child);
	void Draw(unsigned int shaderId, const glm::mat4 &matrix) const;
	void SetTransformationMatrix(const glm::mat4 transformationMatrix) { mTransformationMatrix = transformationMatrix; }

	void Translate(const glm::vec3 &translationVector);
	void Rotate(float angle, const glm::vec3& rotationVector);

private:
	std::unique_ptr<Geometry> mpGeometry = nullptr;
	//abstractizeaza asta mai tarziu din tgafile in random textura
	std::shared_ptr<TGAFile> mpTextureData = nullptr;
	std::unique_ptr<std::list<SceneNodePtr>> mpChildren = nullptr;

	unsigned int VAO = 0;
	unsigned int VBO = 0;

	glm::mat4 mTransformationMatrix;
};

