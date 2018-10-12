#pragma once

#include <fstream>
#include <string>
#include <sstream>
#include <array>
#include <vector>
#include <regex>

#include "MTLFile.h"
#include "SceneNode.h"

class SceneNode;
class Geometry;

//f, v, vt, vn
class ObjFileParser
{
public:
	ObjFileParser(std::string filename);
	~ObjFileParser() = default;

	std::shared_ptr<SceneNode> GetRoot() { return mRootNode; }
	//std::vector<float> GetTheStuffFromTheFace() { return faceStuff; }

private:
	using vec3 = std::array<float, 3>;
	std::vector<vec3> vertices;
	std::vector<vec3> texCoords;
	std::vector<vec3> vertexNormals;

	vec3 ParseVec3(const std::string &line, float defaultValue = 0.f);

	void ParseFace(const std::string &line);
	void Split(const std::string &group, const char token, std::vector<std::string> &groups_out);

	std::unique_ptr<MTLFile> mpActiveMTLFile = nullptr;
	//std::string mActiveGroup = "";
	std::string mActiveMaterial = "";
	std::unique_ptr<Geometry> mCurrentGeometry = nullptr;

	//Change this to a tree
	//std::vector<std::unique_ptr<SceneNode::Geometry>> mAllGeometry;
	std::shared_ptr<SceneNode> mRootNode = nullptr;
	std::shared_ptr<SceneNode> mCurrentNode = nullptr;
};

