#include "LightingTechnique.h"
#include "Lighting.h"

LightingTechnique::LightingTechnique()
{
	//shaderprogram = new ShaderProgram();
	shaderprogram->AddShader("Shaders/lighting2.vs", GL_VERTEX_SHADER);
	shaderprogram->AddShader("Shaders/lighting2.fs", GL_FRAGMENT_SHADER);
	shaderprogram->Finalize();
	shaderprogram->Bind();

	/*GetParamsInShader(shaderProgram->ShaderID(), gSampler, "gSampler");
	GetParamsInShader(shaderProgram->ShaderID(), gWVPLocation, "gWVP");
	GetParamsInShader(shaderProgram->ShaderID(), gWorld, "gWorld");
	GetParamsInShader(shaderProgram->ShaderID(), gLightWVP, "gLightWVP");
	GetParamsInShader(shaderProgram->ShaderID(), gShadowMap, "gShadowMap");
	GetParamsInShader(shaderProgram->ShaderID(), dirLightLocation_Color, "gDirectionalLight.Base.Color");
	GetParamsInShader(shaderProgram->ShaderID(), dirLightLocation_Ambient, "gDirectionalLight.Base.AmbientIntensity");
	GetParamsInShader(shaderProgram->ShaderID(), dirLightLocation_Diffuse, "gDirectionalLight.Base.DiffuseIntensity");
	GetParamsInShader(shaderProgram->ShaderID(), dirLightLocation_Direction, "gDirectionalLight.Direction");
	GetParamsInShader(shaderProgram->ShaderID(), spotLightLocation_Color, "gSpotLights.Base.Base.Color");
	GetParamsInShader(shaderProgram->ShaderID(), spotLightLocation_Ambient, "gSpotLights.Base.Base.AmbientIntensity");
	GetParamsInShader(shaderProgram->ShaderID(), spotLightLocation_Diffuse, "gSpotLights.Base.Base.DiffuseIntensity");
	GetParamsInShader(shaderProgram->ShaderID(), spotLightLocation_Position, "gSpotLights.Base.Position");
	GetParamsInShader(shaderProgram->ShaderID(), spotLightLocation_Constant, "gSpotLights.Base.Atten.Constant");
	GetParamsInShader(shaderProgram->ShaderID(), spotLightLocation_Linear, "gSpotLights.Base.Atten.Linear");
	GetParamsInShader(shaderProgram->ShaderID(), spotLightLocation_Exp, "gSpotLights.Base.Atten.Exp");
	GetParamsInShader(shaderProgram->ShaderID(), spotLightLocation_Direction, "gSpotLights.Direction");
	GetParamsInShader(shaderProgram->ShaderID(), spotLightLocation_Cutoff, "gSpotLights.Cutoff");
	GetParamsInShader(shaderProgram->ShaderID(), gEyeWorldPos, "gEyeWorldPos");
	GetParamsInShader(shaderProgram->ShaderID(), gMatSpecularIntensity, "gMatSpecularIntensity");
	GetParamsInShader(shaderProgram->ShaderID(), gSpecularPower, "gSpecularPower");*/
	gSampler=shaderprogram->GetUniformLocation("gSampler");
	gWVPLocation=shaderprogram->GetUniformLocation("gWVP");
	gWorld=shaderprogram->GetUniformLocation("gWorld");
	gLightWVP=shaderprogram->GetUniformLocation("gLightWVP");
	gShadowMap=shaderprogram->GetUniformLocation("gShadowMap");
	dirLightLocation_Color=shaderprogram->GetUniformLocation("gDirectionalLight.Base.Color");
	dirLightLocation_Ambient=shaderprogram->GetUniformLocation("gDirectionalLight.Base.AmbientIntensity");
	dirLightLocation_Diffuse=shaderprogram->GetUniformLocation("gDirectionalLight.Base.DiffuseIntensity");
	dirLightLocation_Direction=shaderprogram->GetUniformLocation("gDirectionalLight.Direction");
	spotLightLocation_Color=shaderprogram->GetUniformLocation("gSpotLights.Base.Base.Color");
	spotLightLocation_Ambient=shaderprogram->GetUniformLocation("gSpotLights.Base.Base.AmbientIntensity");
	spotLightLocation_Diffuse=shaderprogram->GetUniformLocation("gSpotLights.Base.DiffuseIntensity");
	spotLightLocation_Position= shaderprogram->GetUniformLocation("gSpotLights.Base.Position");
	spotLightLocation_Constant=shaderprogram->GetUniformLocation("gSpotLights.Base.Atten.Constant");
	spotLightLocation_Linear=shaderprogram->GetUniformLocation("gSpotLights.Base.Atten.Linear");
	spotLightLocation_Exp=shaderprogram->GetUniformLocation("gSpotLights.Base.Atten.Exp");
	spotLightLocation_Direction=shaderprogram->GetUniformLocation("gSpotLights.Direction");
	spotLightLocation_Cutoff=shaderprogram->GetUniformLocation("gSpotLights.Cutoff");
	gEyeWorldPos=shaderprogram->GetUniformLocation("gEyeWorldPos");
	gMatSpecularIntensity=shaderprogram->GetUniformLocation("gMatSpecularIntensity");
	gSpecularPower=shaderprogram->GetUniformLocation("gSpecularPower");

}

LightingTechnique::~LightingTechnique()
{
	
}

void LightingTechnique::SetLight(DirectionalLight& directionLight, SpotLight& spotLight)
{
	glUniform3f(dirLightLocation_Color, directionLight.Color.x, directionLight.Color.y, directionLight.Color.z);
	glUniform1f(dirLightLocation_Ambient, directionLight.AmbientIntensity);
	Vector3f Direction = directionLight.Direction;
	Direction.Normalize();
	glUniform3f(dirLightLocation_Direction, Direction.x, Direction.y, Direction.z);
	glUniform1f(dirLightLocation_Diffuse, directionLight.DiffuseIntensity);
	glUniform3f(spotLightLocation_Color, spotLight.Color.x, spotLight.Color.y, spotLight.Color.z);
	glUniform1f(spotLightLocation_Ambient, spotLight.AmbientIntensity);
	glUniform1f(spotLightLocation_Diffuse, spotLight.DiffuseIntensity);
	glUniform3f(spotLightLocation_Position, spotLight.Position.x, spotLight.Position.y, spotLight.Position.z);
	glUniform1f(spotLightLocation_Constant, spotLight.Attenuation.Constant);
	glUniform1f(spotLightLocation_Linear, spotLight.Attenuation.Linear);
	glUniform1f(spotLightLocation_Exp, spotLight.Attenuation.Exp);
	glUniform3f(spotLightLocation_Direction, spotLight.Direction.x, spotLight.Direction.y, spotLight.Direction.z);
	glUniform1f(spotLightLocation_Cutoff, spotLight.Cutoff);
	glUniform1f(gMatSpecularIntensity, 0.5);
	glUniform1f(gSpecularPower, 0.5);
}

void LightingTechnique::SetSampler(const int TexUnit)
{
	glUniform1i(gSampler, TexUnit);
}

void LightingTechnique::SetShadowSampler(const int TexUnit)
{
	glUniform1i(gShadowMap, TexUnit);
}

void LightingTechnique::SetWVP(const Matrix4f& mat)
{
	glUniformMatrix4fv(gWVPLocation, 1, GL_TRUE, (const GLfloat*)mat);
}

void LightingTechnique::SetWorld(const Matrix4f& mat)
{
	glUniformMatrix4fv(gWorld, 1, GL_TRUE, (const GLfloat*)mat);
}

void LightingTechnique::SetLightWVP(const Matrix4f& mat)
{
	glUniformMatrix4fv(gLightWVP, 1, GL_TRUE, (const GLfloat*)mat);
}
