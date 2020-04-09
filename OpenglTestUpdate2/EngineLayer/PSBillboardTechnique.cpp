#include "PSBillboardTechnique.h"
#include "engine_common.h"

PSBillboardTechnique::PSBillboardTechnique()
{
	shaderprogram->AddShader("Shaders/psbillboard.vs", GL_VERTEX_SHADER);
	shaderprogram->AddShader("Shaders/psbillboard.gs", GL_GEOMETRY_SHADER);
	shaderprogram->AddShader("Shaders/psbillboard.fs", GL_FRAGMENT_SHADER);
	shaderprogram->Finalize();
	m_VPLocation = shaderprogram->GetUniformLocation("gVP");
	m_cameraPosLocation = shaderprogram->GetUniformLocation("gCameraPos");
	m_colorMapLocation = shaderprogram->GetUniformLocation("gColorMap");
	m_billboardSizeLocation = shaderprogram->GetUniformLocation("gBillboardSize");

	if (m_VPLocation == INVALID_UNIFORM_LOCATION ||
		m_cameraPosLocation == INVALID_UNIFORM_LOCATION ||
		m_billboardSizeLocation == INVALID_UNIFORM_LOCATION ||
		m_colorMapLocation == INVALID_UNIFORM_LOCATION) 
	{}
}

PSBillboardTechnique::~PSBillboardTechnique()
{

}

void PSBillboardTechnique::SetVP(const Matrix4f& VP)
{
	glUniformMatrix4fv(m_VPLocation, 1, GL_TRUE, (const GLfloat*)VP.m);
}

void PSBillboardTechnique::SetCameraPosition(const Vector3f& Pos)
{
	glUniform3f(m_cameraPosLocation, Pos.x, Pos.y, Pos.z);
}

void PSBillboardTechnique::SetColorTextureUnit(unsigned int TextureUnit)
{
	glUniform1i(m_colorMapLocation, TextureUnit);
}

void PSBillboardTechnique::SetBillboardSize(float BillboardSize)
{
	glUniform1f(m_billboardSizeLocation, BillboardSize);
}
