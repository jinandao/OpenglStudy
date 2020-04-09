#pragma once

#include <glew.h>

class RandomTexture
{
private:
	GLuint textureObj;
public:
	RandomTexture();
	~RandomTexture();
	bool InitRandomTexture(unsigned int Size);
	void Bind(GLenum TextureUnit);
};