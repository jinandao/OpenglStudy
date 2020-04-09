#include "shadowTechnique.h"

ShadowTechnique::ShadowTechnique()
{
	shaderprogram->AddShader("Shaders/shadow_map.vs", GL_VERTEX_SHADER);
	shaderprogram->AddShader("Shaders/shadow_map.fs", GL_FRAGMENT_SHADER);
	shaderprogram->Finalize();
	shaderprogram->Bind();

	m_WVP=shaderprogram->GetUniformLocation("gWVP");
	m_Sampler=shaderprogram->GetUniformLocation("gShadowMap");
}

ShadowTechnique::~ShadowTechnique()
{
}

//void ShadowTechnique::Enable()
//{
//	shaderprogram->Bind();
//}

void ShadowTechnique::SetSampler(const int TexUnit)
{
	glUniform1i(m_Sampler, TexUnit);
}

void ShadowTechnique::SetWVP(const Matrix4f& mat)
{
	glUniformMatrix4fv(m_WVP, 1, GL_TRUE, (const GLfloat*)mat);
}
