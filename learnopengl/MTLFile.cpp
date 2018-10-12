#include "MTLFile.h"

#include <fstream>
#include <sstream>

MTLFile::MTLFile(const std::string &filename) :
	mFilename(filename),
	mpTextureMap(std::make_unique<TGAMap>())
{
	std::ifstream file(filename);
	assert(file.is_open());

	std::string line;

	while (!file.eof())
	{
		std::getline(file, line);

		if (line.empty())
			continue;

		auto tokenValuePair = ParseLine(line);

		if (tokenValuePair.first == "newmtl")
		{
			std::string materialName = tokenValuePair.second;
			
			std::getline(file, line);
			tokenValuePair = ParseLine(line);

			if (tokenValuePair.first == "map_Kd")
			{
				std::shared_ptr<TGAFile> mat = std::make_shared<TGAFile>(tokenValuePair.second);
				mpTextureMap->insert(std::make_pair(materialName, mat));
			}
		}
	}
}

MTLFile::TokenValuePair MTLFile::ParseLine(const std::string &line)
{
	std::istringstream iss(line);

	assert(!line.empty());
	if (line.empty())
	{
		return std::make_pair("ERROR", line);
	}

	std::string token;
	iss >> token;
	std::string value = line.substr(token.length() + 1);

	return std::make_pair(token, value);
}

