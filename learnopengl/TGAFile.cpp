#include "TGAFile.h"

#include <vector>
#include <iostream>

TGAFile::TGAFile(std::string filename)
{
	std::ifstream file(filename, std::ifstream::binary);

	if (file)
	{
		file.read((char*)(&header), sizeof(header));
		file.close();
		assert(header.idLength == 0);
		assert(header.colorMapType == 0);
		assert(header.imageSpecification.pixelDepth == 24 || header.imageSpecification.pixelDepth == 32);
		//std::cout << sizeof(header) << " " << &header << " " << sizeof(header.colorMapSpecification) << " " << &header.colorMapSpecification.firstEntryIndex << " " << sizeof(header.imageSpecification) << " " << &header.imageSpecification.xOrigin << std::endl;
		unsigned int imageSize = header.imageSpecification.width * header.imageSpecification.height * header.imageSpecification.pixelDepth / 8;
		imageData = new char[imageSize];
		file.read(imageData, imageSize);
		file.close();
	}
}


TGAFile::~TGAFile()
{
	if (imageData != nullptr)
	{
		delete[] imageData;
	}
}
