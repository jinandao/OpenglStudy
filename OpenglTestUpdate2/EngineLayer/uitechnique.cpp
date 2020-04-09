#include "uitechnique.h"
#include "OpenglTools.h"

UITechnique::UITechnique()
{
	shaderprogram->AddShader("Shaders/uishader.vs", GL_VERTEX_SHADER);
	shaderprogram->AddShader("Shaders/uishader.fs", GL_FRAGMENT_SHADER);
	shaderprogram->Finalize();

	shaderprogram->Bind();
	m_UITex = shaderprogram->GetUniformLocation("gDeltaTimeMillis");
	if (m_UITex == 0xFFFFFFFF)
	{
		std::cout << "sampler in uitehnique not found" << std::endl;
	}
}

UITechnique::~UITechnique()
{
	
}

void UITechnique::SetSampler(int TexUnit)
{
	glUniform1i(m_UITex, 0);
}
