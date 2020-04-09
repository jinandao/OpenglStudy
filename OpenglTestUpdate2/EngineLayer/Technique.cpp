#include "Technique.h"

Technique::Technique()
{
	shaderprogram = new ShaderProgram();
}

Technique::~Technique()
{
	if (shaderprogram != nullptr)
	{
		delete shaderprogram;
		shaderprogram = nullptr;
	}
}

void Technique::Enable()
{
	shaderprogram->Bind();
}
