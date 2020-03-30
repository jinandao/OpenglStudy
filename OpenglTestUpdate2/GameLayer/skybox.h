#pragma once

#include "Camera.h"
#include "../EngineLayer/cubemap_texture.h"
#include "../EngineLayer/texture.h"
#include "../EngineLayer/mesh.h"

class SkyBox
{
private:
	const Camera* m_pCamera;
	CubemapTexture* m_pCubemapTex;
	PersProjInfo m_persProjInfo;
	Mesh* m_pMesh;

public:
	SkyBox(const Camera* pCamera, const PersProjInfo& p);
	~SkyBox();
	bool Init(const string& Directory,
		const string& PosXFilename,
		const string& NegXFilename,
		const string& PosYFilename,
		const string& NegYFilename,
		const string& PosZFilename,
		const string& NegZFilename);
	void Render();

};