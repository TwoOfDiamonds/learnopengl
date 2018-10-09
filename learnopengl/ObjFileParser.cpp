#include "ObjFileParser.h"

#include <iostream>

ObjFileParser::ObjFileParser(std::string filename)
{
	vertices.push_back(vec3{ 0.f, 0.f, 0.f });
	texCoords.push_back(vec3{ 0.f, 0.f, 0.f });
	vertexNormals.push_back(vec3{ 0.f, 0.f, 0.f });

	std::ifstream file(filename);
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
	}

	float minx = vertices[1][0], miny = vertices[1][1], minz = vertices[1][2];
	float maxx = vertices[1][0], maxy = vertices[1][1], maxz = vertices[1][2];

	for (int i = 2; i < vertices.size(); i++)
	{
		if (minx > vertices[i][0])
			minx = vertices[i][0];

		if (miny > vertices[i][1])
			miny = vertices[i][1];

		if (minz > vertices[i][2])
			minz = vertices[i][2];

		if (maxx < vertices[i][0])
			maxx = vertices[i][0];

		if (maxy < vertices[i][1])
			maxy = vertices[i][1];

		if (maxz < vertices[i][2])
			maxz = vertices[i][2];
	}

	std::cout << "min " << minx << " " << miny << " " << minz << std::endl;
	std::cout << "max " << maxx << " " << maxy << " " << maxz << std::endl;

	file.close();
}

ObjFileParser::~ObjFileParser()
{
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

		faceStuff.push_back(vertices[vertexIndex][0]);
		faceStuff.push_back(vertices[vertexIndex][1]);
		faceStuff.push_back(vertices[vertexIndex][2]);

		faceStuff.push_back(texCoords[textureCoordIndex][0]);
		faceStuff.push_back(texCoords[textureCoordIndex][1]);
		faceStuff.push_back(texCoords[textureCoordIndex][2]);

		faceStuff.push_back(vertexNormals[normalIndex][0]);
		faceStuff.push_back(vertexNormals[normalIndex][1]);
		faceStuff.push_back(vertexNormals[normalIndex][2]);
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
