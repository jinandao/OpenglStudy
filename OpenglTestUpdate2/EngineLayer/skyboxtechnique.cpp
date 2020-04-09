#include "skyboxtechnique.h"

SkyBoxTechnique::SkyBoxTechnique()
{
	//shaderprogram = new ShaderProgram();
	shaderprogram->AddShader("Shaders/skybox.vs", GL_VERTEX_SHADER);
	shaderprogram->AddShader("Shaders/skybox.fs", GL_FRAGMENT_SHADER);
	shaderprogram->Finalize();

	shaderprogram->Bind();
	//TODO 参数获取
	m_WVP=shaderprogram->GetUniformLocation("gWVP");
	m_Sampler=shaderprogram->GetUniformLocation("gCubemapTexture");
}

SkyBoxTechnique::~SkyBoxTechnique()
{
}

//void SkyBoxTechnique::Enable()
//{
//	shaderprogram->Bind();
//}

void SkyBoxTechnique::SetSampler(const int TexUnit)
{
	glUniform1i(m_Sampler, TexUnit);
}

void SkyBoxTechnique::SetWVP(const Matrix4f& mat)
{
	glUniformMatrix4fv(m_WVP, 1, GL_TRUE, (const GLfloat*)mat);
}