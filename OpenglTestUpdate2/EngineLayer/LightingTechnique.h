#pragma once

//#include "Shader.h"
#include "Technique.h"
#include "MathTools.h"
#include <glew.h>

class DirectionalLight;
class SpotLight;

class LightingTechnique:public Technique
{
private:
	//ShaderProgram* shaderprogram;
	GLuint gSampler;
	GLuint gWVPLocation;
	GLuint gShadowMap;
	GLuint dirLightLocation_Color;
	GLuint dirLightLocation_Ambient;
	GLuint dirLightLocation_Diffuse;
	GLuint dirLightLocation_Direction;
	GLuint spotLightLocation_Color;
	GLuint spotLightLocation_Ambient;
	GLuint spotLightLocation_Diffuse;
	GLuint spotLightLocation_Position;
	GLuint spotLightLocation_Constant;
	GLuint spotLightLocation_Linear;
	GLuint spotLightLocation_Exp;
	GLuint spotLightLocation_Direction;
	GLuint spotLightLocation_Cutoff;
	GLuint gWorld;
	GLuint gLightWVP;
	GLuint gEyeWorldPos;
	GLuint gMatSpecularIntensity;
	GLuint gSpecularPower;

public:
	LightingTechnique();
	~LightingTechnique();

	void SetLight(DirectionalLight& directionLight,SpotLight& spotLight);
	void SetSampler(const int TexUnit);
	void SetShadowSampler(const int TexUnit);
	void SetWVP(const Matrix4f& mat);
	void SetWorld(const Matrix4f& mat);
	void SetLightWVP(const Matrix4f& mat);
};