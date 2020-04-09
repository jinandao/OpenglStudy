#pragma once
#include "Shader.h"

class Technique
{
protected:
	ShaderProgram* shaderprogram;
public:
	Technique();
	virtual ~Technique();
	void Enable();
};