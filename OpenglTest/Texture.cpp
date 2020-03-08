#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include <STB/stb_image.h>
#include <glew.h>
#include <iostream>

Texture::Texture(unsigned int ID, const std::string& path) :rendererID(ID), filePath(path), localBuffer(nullptr), width(0), height(0), bPP(0)
{
	stbi_set_flip_vertically_on_load(1);
	localBuffer = stbi_load(path.c_str(), &width, &height, &bPP, 4);//!

	//std::cout << "gen picture " << rendererID << std::endl;
	glGenTextures(1, &rendererID);//!?
	glBindTexture(GL_TEXTURE_2D, rendererID);//?

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//?
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	std::cout << "width:" << width << " height:" << height << std::endl;
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, localBuffer);//将图片转移到纹理？
	glBindTexture(GL_TEXTURE_2D, 0);

	if (localBuffer)
	{
		stbi_image_free(localBuffer);
	}
}

Texture::~Texture()
{
	glDeleteTextures(1, &rendererID);
}

void Texture::Bind(unsigned int slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, rendererID);
}

void Texture::Unbind() const
{
	glBindTexture(GL_TEXTURE_2D,0);
}