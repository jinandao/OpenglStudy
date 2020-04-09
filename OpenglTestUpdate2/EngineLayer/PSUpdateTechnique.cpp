#include "PSUpdateTechnique.h"
#include "engine_common.h"

PSUpdateTechnique::PSUpdateTechnique()
{
	//shaderprogram = new ShaderProgram();
	shaderprogram->AddShader("Shaders/ps_update.vs", GL_VERTEX_SHADER);
	shaderprogram->AddShader("Shaders/ps_update.gs", GL_GEOMETRY_SHADER);
	shaderprogram->AddShader("Shaders/ps_update.fs", GL_FRAGMENT_SHADER);
	//shaderprogram->Finalize();

	const GLchar* Varyings[4];
	Varyings[0] = "Type1";
	Varyings[1] = "Position1";
	Varyings[2] = "Velocity1";
	Varyings[3] = "Age1";

	glTransformFeedbackVaryings(shaderprogram->ShaderID(), 4, Varyings, GL_INTERLEAVED_ATTRIBS);

	shaderprogram->Finalize();

	m_deltaTimeMillisLocation = shaderprogram->GetUniformLocation("gDeltaTimeMillis");
	m_randomTextureLocation = shaderprogram->GetUniformLocation("gRandomTexture");
	m_timeLocation = shaderprogram->GetUniformLocation("gTime");
	m_launcherLifetimeLocation = shaderprogram->GetUniformLocation("gLauncherLifetime");
	m_shellLifetimeLocation = shaderprogram->GetUniformLocation("gShellLifetime");
	m_secondaryShellLifetimeLocation = shaderprogram->GetUniformLocation("gSecondaryShellLifetime");

	if (m_deltaTimeMillisLocation == INVALID_UNIFORM_LOCATION ||
		m_timeLocation == INVALID_UNIFORM_LOCATION ||
		m_randomTextureLocation == INVALID_UNIFORM_LOCATION ||
		m_launcherLifetimeLocation == INVALID_UNIFORM_LOCATION ||
		m_shellLifetimeLocation == INVALID_UNIFORM_LOCATION ||
		m_secondaryShellLifetimeLocation == INVALID_UNIFORM_LOCATION) 
	{
		std::cout << "something not found" << std::endl;
	}
}

PSUpdateTechnique::~PSUpdateTechnique()
{
	
}

void PSUpdateTechnique::SetParticleLifetime(float Lifetime)
{
}

void PSUpdateTechnique::SetDeltaTimeMillis(unsigned int DeltaTimeMillis)
{
	glUniform1f(m_deltaTimeMillisLocation, (float)DeltaTimeMillis);
}

void PSUpdateTechnique::SetTime(int Time)
{
	glUniform1f(m_timeLocation, (float)Time);
}

void PSUpdateTechnique::SetRandomTextureUnit(unsigned int TextureUnit)
{
	glUniform1i(m_randomTextureLocation, TextureUnit);
}

void PSUpdateTechnique::SetLauncherLifetime(float Lifetime)
{
	glUniform1f(m_launcherLifetimeLocation, Lifetime);
}

void PSUpdateTechnique::SetShellLifetime(float Lifetime)
{
	glUniform1f(m_shellLifetimeLocation, Lifetime);
}

void PSUpdateTechnique::SetSecondaryShellLifetime(float Lifetime)
{
	glUniform1f(m_secondaryShellLifetimeLocation, Lifetime);
}
