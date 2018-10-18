#pragma once

#include <map>
#include <string>

#include "TGAFile.h"

class MTLFile
{
public:
	MTLFile(const std::string &filename);
	~MTLFile() = default;

	std::string GetName() { return mFilename; }
	//std::shared_ptr<TGAFile> operator[](const std::string& materialName) { return mpTextureMap->at(materialName); }
	std::shared_ptr<TGAFile> GetMaterial(const std::string& materialName) { return materialName.empty() ? nullptr : mpTextureMap->at(materialName); }
private:
	std::string mFilename = "";

	using TGAMap = std::map<std::string, std::shared_ptr<TGAFile>>;
	std::unique_ptr<TGAMap> mpTextureMap = nullptr;

	using TokenValuePair = std::pair<std::string, std::string>;
	TokenValuePair ParseLine(const std::string &line);
};

