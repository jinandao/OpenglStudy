#pragma once
#include <glew.h>
//#include "Shader.h"
#include "Technique.h"

class UITechnique:public Technique
{
private:
	//ShaderProgram* shaderprogram;
	GLuint m_UITex;

public:
	UITechnique();
	~UITechnique();
	//void Enable();
	void SetSampler(int TexUnit);
};