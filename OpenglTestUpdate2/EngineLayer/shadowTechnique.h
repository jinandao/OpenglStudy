#pragma once

#include <glew.h>
#include "MathTools.h"
//#include "Shader.h"
#include "Technique.h"

class ShadowTechnique:public Technique
{
private:
	//ShaderProgram* shaderprogram;
	GLuint m_Sampler;
	GLuint m_WVP;

public:
	ShadowTechnique();
	~ShadowTechnique();
	//void Enable();
	void SetSampler(const int TexUnit);
	void SetWVP(const Matrix4f& mat);
	
};