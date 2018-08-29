#pragma once

#include <fstream>
#include <string>
#include <cassert>

class TGAFile
{
public:
	TGAFile(std::string filename);
	~TGAFile();

	unsigned int GetWidth() { return header.imageSpecification.width; };
	unsigned int GetHeight() { return header.imageSpecification.height; };
	char* GetImageData() 
	{ 
		assert(imageData != nullptr);
		return imageData; 
	};
private:

	struct Header
	{
		unsigned __int8 idLength;
		unsigned __int8 colorMapType;
		unsigned __int8 imageType;

		struct ColorMapSpecification
		{
			unsigned __int16 firstEntryIndex;
			unsigned __int16 colorMapLength;
			unsigned __int8 entrySize;
		} colorMapSpecification;

		struct ImageSpecification
		{
			unsigned __int16 xOrigin;
			unsigned __int16 yOrigin;
			unsigned __int16 width;
			unsigned __int16 height;
			unsigned __int8 pixelDepth;
			unsigned __int8 imageDescriptor;
		} imageSpecification;
	} header;

	char* imageData = nullptr;

};

