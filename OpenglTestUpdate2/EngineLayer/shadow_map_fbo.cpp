#include "shadow_map_fbo.h"
#include <iostream>

bool ShadowMapFBO::Init(unsigned int WindowWidth, unsigned int WindowHeight)
{
	// Create the FBO
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	// Create the depth buffer
	glGenTextures(1, &shadowMap);
	glBindTexture(GL_TEXTURE_2D, shadowMap);
	
	/*glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, WindowWidth, WindowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap, 0);*/
	// Disable writes to the color buffer
	//glDrawBuffer(GL_NONE);
	//glReadBuffer(GL_NONE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);   
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);    
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);    
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WindowWidth, WindowHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);	
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, shadowMap, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	
	GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (Status != GL_FRAMEBUFFER_COMPLETE) {
		std::cout<<"FB error, status: "<<Status;
		return false;
	}

	return true;
}

//bool ShadowMapFBO::Init(unsigned int WindowWidth, unsigned int WindowHeight)
//{
//	glGenFramebuffers(1, &fbo);
//	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
//
//	glGenTextures(3, m_textures);
//	glGenTextures(1, &shadowMap);
//
//	for (unsigned int i = 0; i < 3; i++)
//	{
//		glBindTexture(GL_TEXTURE_2D, m_textures[i]);
//		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, WindowWidth, WindowHeight, 0, GL_RGB, GL_FLOAT, NULL);
//		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_textures[i], 0);
//	}
//
//	//depth
//	glBindTexture(GL_TEXTURE_2D, shadowMap);
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, WindowWidth, WindowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
//	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap, 0);
//
//	//color
//	//glBindTexture(GL_TEXTURE_2D, shadowMap);
//	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WindowWidth, WindowHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
//	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, shadowMap, 0);
//	
//	GLenum DrawBuffers[] = { GL_COLOR_ATTACHMENT0,
//							GL_COLOR_ATTACHMENT1,
//							GL_COLOR_ATTACHMENT2 };
//	glDrawBuffers(3, DrawBuffers);
//
//	GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
//
//	if (Status != GL_FRAMEBUFFER_COMPLETE) {
//		printf("FB error, status: 0x%x\n", Status);
//		return false;
//	}
//	
//	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
//	
//	return true;
//}