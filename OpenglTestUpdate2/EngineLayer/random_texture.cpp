#include "random_texture.h"
#include "MathTools.h"

RandomTexture::RandomTexture()
{
	textureObj = 0;
}

RandomTexture::~RandomTexture()
{
	if (textureObj != 0)
	{
		glDeleteTextures(1, &textureObj);
	}
}

bool RandomTexture::InitRandomTexture(unsigned int Size)
{
	Vector3f* pRandomData = new Vector3f[Size];
	for (unsigned int i = 0; i < Size; i++)
	{
		pRandomData[i].x = RandomFloat();
		pRandomData[i].y = RandomFloat();
		pRandomData[i].z = RandomFloat();
	}
	glGenTextures(1, &textureObj);
	glBindTexture(GL_TEXTURE_1D, textureObj);
	glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB, Size, 0, GL_RGB, GL_FLOAT, pRandomData);
	glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	return true;
}

void RandomTexture::Bind(GLenum TextureUnit)
{
	glActiveTexture(TextureUnit);
	glBindTexture(GL_TEXTURE_1D, textureObj);
}
