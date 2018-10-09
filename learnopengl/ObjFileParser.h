#pragma once

#include <fstream>
#include <string>
#include <sstream>
#include <array>
#include <vector>
#include <regex>


//f, v, vt, vn
class ObjFileParser
{
public:
	ObjFileParser(std::string filename);
	~ObjFileParser();

	std::vector<float> GetTheStuffFromTheFace() { return faceStuff; }

private:
	using vec3 = std::array<float, 3>;
	std::vector<vec3> vertices;
	std::vector<vec3> texCoords;
	std::vector<vec3> vertexNormals;

	vec3 ParseVec3(const std::string &line, float defaultValue = 0.f);

	std::vector<float> faceStuff;

	void ParseFace(const std::string &line);
	void Split(const std::string &group, const char token, std::vector<std::string> &groups_out);
};

