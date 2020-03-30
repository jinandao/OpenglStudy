#include "Texture.h"
#include <iostream>
#include <string>
#define STB_IMAGE_IMPLEMENTATION
#include <STB/stb_image.h>

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
	std::cout << "path: "<<path<<" "<<"width:" << width << " height:" << height << std::endl;
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


static const GLenum types[6] = { GL_TEXTURE_CUBE_MAP_POSITIVE_X,
								  GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
								  GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
								  GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
								  GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
								  GL_TEXTURE_CUBE_MAP_NEGATIVE_Z };

CubemapTexture::CubemapTexture(const string& Directory,
	const string& PosXFilename,
	const string& NegXFilename,
	const string& PosYFilename,
	const string& NegYFilename,
	const string& PosZFilename,
	const string& NegZFilename)
{
	//string::const_iterator it = Directory.end();
	//it--;
	//string BaseDir = (*it == '/') ? Directory : Directory + "/";
	string BaseDir = Directory;

	m_fileNames[0] = BaseDir + PosXFilename;
	m_fileNames[1] = BaseDir + NegXFilename;
	m_fileNames[2] = BaseDir + PosYFilename;
	m_fileNames[3] = BaseDir + NegYFilename;
	m_fileNames[4] = BaseDir + PosZFilename;
	m_fileNames[5] = BaseDir + NegZFilename;

	m_textureObj = 0;
}

CubemapTexture::~CubemapTexture()
{
	if (m_textureObj != 0) {
		glDeleteTextures(1, &m_textureObj);
	}
}

bool CubemapTexture::Load()
{
	glGenTextures(1, &m_textureObj);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureObj);

	for (unsigned int i = 0; i < (sizeof(types) / sizeof(types[0])); i++)
	{
		int m_Width, m_Height, m_BPP;
		stbi_set_flip_vertically_on_load(0);
		unsigned char* m_LocalBuffer = stbi_load(m_fileNames[i].c_str(), &m_Width, &m_Height, &m_BPP, 4);
		std::cout << "cubemap: "<< m_fileNames[i]<<" " << m_Width << " " << m_Height << std::endl;
		glTexImage2D(types[i], 0, GL_RGB, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		if (m_LocalBuffer)
		{
			stbi_image_free(m_LocalBuffer);
		}
	}

	return true;
}

void CubemapTexture::Bind(GLenum TextureUnit)
{
	glActiveTexture(TextureUnit);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureObj);
}