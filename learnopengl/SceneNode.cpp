#include "SceneNode.h"

#include <iostream>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>

SceneNode::SceneNode(const SceneNode & otherSceneNode) :
	mpGeometry(otherSceneNode.mpGeometry != nullptr ? std::make_unique<Geometry>(*(otherSceneNode.mpGeometry)) : nullptr),
	mpTextureData(otherSceneNode.mpTextureData != nullptr ? std::make_shared<TGAFile>(*(otherSceneNode.mpTextureData)) : nullptr),
	mpChildren(otherSceneNode.mpChildren != nullptr ? std::make_unique<std::list<SceneNodePtr>>(*(otherSceneNode.mpChildren)) : nullptr),
	mTransformationMatrix(1.0f)
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	if (mpGeometry != nullptr)
	{
		const auto& vertices = mpGeometry->allVertices;

		//creating vertex buffer object
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)12);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)24);
		glEnableVertexAttribArray(2);
	}
}

SceneNode::SceneNode(SceneNode && otherSceneNode) :
	mpGeometry(std::move(otherSceneNode.mpGeometry)),
	mpTextureData(std::move(otherSceneNode.mpTextureData)),
	mpChildren(std::move(otherSceneNode.mpChildren)),
	mTransformationMatrix(std::move(otherSceneNode.mTransformationMatrix))
{
	VAO = otherSceneNode.VAO;
	VBO = otherSceneNode.VBO;
}

SceneNode::SceneNode(std::unique_ptr<Geometry> geometry, std::shared_ptr<TGAFile> textureData) :
	mpGeometry(std::move(geometry)),
	mpTextureData(textureData),
	mpChildren(std::make_unique<std::list<SceneNodePtr>>()),
	mTransformationMatrix(1.0f)
{
	if (mpGeometry != nullptr)
	{
		//creating vertex attribute object
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		const auto& vertices = mpGeometry->allVertices;

		//creating vertex buffer object
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)12);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)24);
		glEnableVertexAttribArray(2);
	}
}

void SceneNode::AttachChild(SceneNodePtr child)
{
	mpChildren->push_back(child);
}

void SceneNode::Draw(unsigned int shaderId, const glm::mat4 &matrix) const
{
	glm::mat4 currentMatrix = matrix * mTransformationMatrix;

	glUniformMatrix4fv(glGetUniformLocation(shaderId, "transMat"), 1, GL_FALSE, glm::value_ptr(currentMatrix));

	if (mpTextureData != nullptr)
	{
		glActiveTexture(GL_TEXTURE0);
		glUniform1i(glGetUniformLocation(shaderId, "texture0"), 0);
		glBindTexture(GL_TEXTURE_2D, mpTextureData->GetTextureId());
	}

	if (mpGeometry != nullptr)
	{
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, mpGeometry->allVertices.size() / 9);
		glBindVertexArray(0);
	}

	for (std::list<SceneNodePtr>::const_iterator it = mpChildren->begin(); it != mpChildren->end(); it++)
	{
		(*it)->Draw(shaderId, currentMatrix);
	}
}

void SceneNode::Translate(const glm::vec3 & translationVector)
{
	mTransformationMatrix = glm::translate(mTransformationMatrix, translationVector);
}

void SceneNode::Rotate(float angle, const glm::vec3& rotationVector)
{
	mTransformationMatrix = glm::rotate(mTransformationMatrix, angle, rotationVector);
}