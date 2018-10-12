#pragma once

#include <glad\glad.h>
#include <list>
#include <memory>

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
	SceneNode(std::unique_ptr<Geometry> geometry = nullptr, std::shared_ptr<TGAFile> textureData = nullptr);
	~SceneNode() = default;

	using SceneNodePtr = std::shared_ptr<SceneNode>;

	void AttachChild(SceneNodePtr child);
	void Draw(unsigned int shaderId) const;

private:
	std::unique_ptr<Geometry> mpGeometry = nullptr;
	//abstractizeaza asta mai tarziu
	std::shared_ptr<TGAFile> mpTextureData = nullptr;
	std::unique_ptr<std::list<SceneNodePtr>> mpChildren = nullptr;

	unsigned int VAO = 0;
	unsigned int VBO = 0;
};

