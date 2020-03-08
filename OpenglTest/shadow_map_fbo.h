#pragma once
#include <glew.h>

class ShadowMapFBO
{
private:
	GLuint fbo;
	GLuint shadowMap;

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
		glActiveTexture(GL_TEXTURE1+TextureUnit);
		glBindTexture(GL_TEXTURE_2D, shadowMap);
	}
};

