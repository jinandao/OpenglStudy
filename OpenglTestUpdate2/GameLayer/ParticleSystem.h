#pragma once

#include <glew.h>

#include "../EngineLayer/random_texture.h"
#include "../EngineLayer/Texture.h"
#include "../EngineLayer/MathTools.h"
#include "../EngineLayer/PSUpdateTechnique.h"
#include "../EngineLayer/PSBillboardTechnique.h"

class ParticleSystem
{
private:
	bool m_isFirst;
	unsigned int m_currVB;
	unsigned int m_currTFB;
	GLuint m_particleBuffer[2];
	GLuint m_transformFeedback[2];
	PSUpdateTechnique* m_updateTechnique;
	PSBillboardTechnique* m_billboardTechnique;
	RandomTexture m_randomTexture;
	Texture* m_pTexture;
	int m_time;

	void UpdateParticles(int DeltaTimeMillis);
	void RenderParticles(const Matrix4f& VP, const Vector3f& CameraPos);

public:
	ParticleSystem();
	~ParticleSystem();
	bool InitParticleSystem(const Vector3f& Pos);
	void SetTechnique(PSUpdateTechnique* ps, PSBillboardTechnique* pb);
	void Render(int DeltaTimeMillis, const Matrix4f& VP, const Vector3f& CameraPos);
};

struct ParticleElement
{
public:
	int times;
	Vector3f pos;
	ParticleSystem* ps;
};