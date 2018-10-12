#include "ObjFileParser.h"

#include <iostream>

ObjFileParser::ObjFileParser(std::string filename) :
	mRootNode(std::make_shared<SceneNode>())
{
	vertices.push_back(vec3{ 0.f, 0.f, 0.f });
	texCoords.push_back(vec3{ 0.f, 0.f, 0.f });
	vertexNormals.push_back(vec3{ 0.f, 0.f, 0.f });

	std::ifstream file(filename);
	assert(file.is_open());

	std::string line;

	while (!file.eof())
	{
		std::getline(file, line);
		std::istringstream iss(line);

		if (line.empty())
			continue;

		std::string token;
		iss >> token;

		line = line.substr(token.length() + 1);

		if (token == "#")
			continue;
		else if (token == "v")
			vertices.push_back(ParseVec3(line));
		else if (token == "vt")
			texCoords.push_back(ParseVec3(line));
		else if (token == "vn")
			vertexNormals.push_back(ParseVec3(line));
		else if (token == "f")
			ParseFace(line);
		else if (token == "g")
		{
			if (mCurrentGeometry != nullptr)
			{
				mRootNode->AttachChild(std::make_shared<SceneNode>(std::move(mCurrentGeometry), mpActiveMTLFile->GetMaterial(mActiveMaterial)));
			}

			mCurrentGeometry = std::make_unique<Geometry>();
			mCurrentGeometry->groupName = line;
		}
		else if (token == "mtllib")
		{
			if (mpActiveMTLFile == nullptr || (mpActiveMTLFile != nullptr && mpActiveMTLFile->GetName() != line))
			{
				mpActiveMTLFile = std::make_unique<MTLFile>(line);
			}
		}
		else if (token == "usemtl")
		{
			//load material
			mActiveMaterial = line;
		}
	}

	if (mCurrentGeometry != nullptr)
	{
		mRootNode->AttachChild(std::make_shared<SceneNode>(std::move(mCurrentGeometry), mpActiveMTLFile->GetMaterial(mActiveMaterial)));
	}

	file.close();
}

ObjFileParser::vec3 ObjFileParser::ParseVec3(const std::string & line, float defaultValue)
{
	vec3 result;

	std::istringstream iss(line);

	int i = 0;
	float v;

	while (iss >> v)
	{
		result[i] = v;
		i++;
	}

	for (; i < result.size(); i++)
		result[i] = defaultValue;

	return result;
}

void ObjFileParser::ParseFace(const std::string & line)
{
	std::vector<std::string> groups;
	Split(line, ' ', groups);

	for (auto& group : groups)
	{
		std::vector<std::string> smallGroups;
		Split(group, '/', smallGroups);

		int vertexIndex = std::stoi(smallGroups[0]);
		if (vertexIndex < 0)
			vertexIndex = vertices.size() + vertexIndex;
		
		int textureCoordIndex = 0;
		if (smallGroups.size() > 1 && !smallGroups[1].empty())
		{
			textureCoordIndex = std::stoi(smallGroups[1]);

			if (textureCoordIndex < 0)
				textureCoordIndex = texCoords.size() + textureCoordIndex;
		}
		
		int normalIndex = 0;
		if (smallGroups.size() > 2 && !smallGroups[2].empty())
		{
			normalIndex = std::stoi(smallGroups[2]);

			if (normalIndex < 0)
				normalIndex = vertexNormals.size() + normalIndex;
		}

		mCurrentGeometry->allVertices.push_back(vertices[vertexIndex][0]);
		mCurrentGeometry->allVertices.push_back(vertices[vertexIndex][1]);
		mCurrentGeometry->allVertices.push_back(vertices[vertexIndex][2]);

		mCurrentGeometry->allVertices.push_back(texCoords[textureCoordIndex][0]);
		mCurrentGeometry->allVertices.push_back(texCoords[textureCoordIndex][1]);
		mCurrentGeometry->allVertices.push_back(texCoords[textureCoordIndex][2]);

		mCurrentGeometry->allVertices.push_back(vertexNormals[normalIndex][0]);
		mCurrentGeometry->allVertices.push_back(vertexNormals[normalIndex][1]);
		mCurrentGeometry->allVertices.push_back(vertexNormals[normalIndex][2]);
	}
}

void ObjFileParser::Split(const std::string &group, const char token, std::vector<std::string> &groups_out)
{
	std::istringstream iss(group);
	std::string smallGroup;

	while (getline(iss, smallGroup, token))
	{
		groups_out.push_back(smallGroup);
	}
}
