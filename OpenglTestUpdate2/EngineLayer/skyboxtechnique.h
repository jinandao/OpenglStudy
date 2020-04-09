#pragma once

#include <glew.h>
#include "Shader.h"
#include "Technique.h"
#include "MathTools.h"

class SkyBoxTechnique:public Technique
{
private:
	//ShaderProgram* shaderprogram;
	GLuint m_Sampler;
	GLuint m_WVP;

public:
	SkyBoxTechnique();
	~SkyBoxTechnique();
	//void Enable();
	void SetSampler(const int TexUnit);
	void SetWVP(const Matrix4f& mat);
};