#pragma once
#include <glew.h>

class ShadowMapFBO
{
public:
	/*enum GBUFFER_TEXTURE_TYPE
	{
		GBUFFER_TEXTURE_TYPE_POSITION,
		GBUFFER_TEXTURE_TYPE_DIFFUSE,
		GBUFFER_TEXTURE_TYPE_NORMAL,
		GBUFFER_NUM_TEXTURES
	};*/

public:
	GLuint fbo;
	GLuint shadowMap;
	//GLuint m_textures[GBUFFER_NUM_TEXTURES];

public:
	ShadowMapFBO() :fbo(0), shadowMap(0) {};
	~ShadowMapFBO()
	{
		if (fbo != 0)
		{
			glDeleteFramebuffers(1, &fbo);
		}
		if (shadowMap != 0)
		{
			glDeleteTextures(1, &shadowMap);
		}
	}

	bool Init(unsigned int WindowWidth, unsigned int WindowHeight);
	void BindForWriting()
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
	}
	void BindForReading(GLenum TextureUnit)
	{
		glActiveTexture(GL_TEXTURE0+TextureUnit);
		glBindTexture(GL_TEXTURE_2D, shadowMap);
	}
	/*void SetReadBuffer(GBUFFER_TEXTURE_TYPE TextureType)
	{
		glReadBuffer(GL_COLOR_ATTACHMENT0 + TextureType);
	}*/
};

