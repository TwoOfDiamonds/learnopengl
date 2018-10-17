#include "TGAFile.h"

#include <glad\glad.h>
#include <vector>
#include <iostream>

TGAFile::TGAFile(const TGAFile & otherTga)
{
	header = otherTga.header;

	unsigned int imageSize = header.imageSpecification.width * header.imageSpecification.height * header.imageSpecification.pixelDepth / 8;
	imageData = new char[imageSize];

	memcpy_s(imageData, imageSize, otherTga.imageData, imageSize);

	glGenTextures(1, &textureid);
	glBindTexture(GL_TEXTURE_2D, textureid);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int format = GetPixelDepth() == 32 ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_2D, 0, format, GetWidth(), GetHeight(), 0, format, GL_UNSIGNED_BYTE, GetImageData());
}

TGAFile::TGAFile(std::string filename)
{
	std::ifstream file(filename, std::ifstream::binary);

	assert(file.is_open());

	if (file)
	{
		file.read((char*)(&header), sizeof(header));
		assert(header.idLength == 0);
		assert(header.colorMapType == 0);
		assert(header.imageSpecification.pixelDepth == 24 || header.imageSpecification.pixelDepth == 32);
		//std::cout << sizeof(header) << " " << &header << " " << sizeof(header.colorMapSpecification) << " " << &header.colorMapSpecification.firstEntryIndex << " " << sizeof(header.imageSpecification) << " " << &header.imageSpecification.xOrigin << std::endl;

		unsigned int imageSize = header.imageSpecification.width * header.imageSpecification.height * header.imageSpecification.pixelDepth / 8;
		imageData = new char[imageSize];
		file.read(imageData, imageSize);
		file.close();
	}

	glGenTextures(1, &textureid);
	glBindTexture(GL_TEXTURE_2D, textureid);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int format = GetPixelDepth() == 32 ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_2D, 0, format, GetWidth(), GetHeight(), 0, format, GL_UNSIGNED_BYTE, GetImageData());
}


TGAFile::~TGAFile()
{
	if (imageData != nullptr)
	{
		delete[] imageData;
	}
}
