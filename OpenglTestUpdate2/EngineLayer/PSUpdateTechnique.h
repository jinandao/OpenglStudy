#pragma once

//#include "Shader.h"
#include "Technique.h"

class PSUpdateTechnique:public Technique
{
private:
	//ShaderProgram* shaderprogram;
	GLuint m_deltaTimeMillisLocation;
	GLuint m_randomTextureLocation;
	GLuint m_timeLocation;
	GLuint m_launcherLifetimeLocation;
	GLuint m_shellLifetimeLocation;
	GLuint m_secondaryShellLifetimeLocation;

public:
	PSUpdateTechnique();
	~PSUpdateTechnique();
	void SetParticleLifetime(float Lifetime);
	void SetDeltaTimeMillis(unsigned int DeltaTimeMillis);
	void SetTime(int Time);
	void SetRandomTextureUnit(unsigned int TextureUnit);
	void SetLauncherLifetime(float Lifetime);
	void SetShellLifetime(float Lifetime);
	void SetSecondaryShellLifetime(float Lifetime);
};