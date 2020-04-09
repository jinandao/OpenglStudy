#pragma once

//#include "Shader.h"
#include "Technique.h"
#include "MathTools.h"

class PSBillboardTechnique:public Technique
{
private:
	GLuint m_VPLocation;
	GLuint m_cameraPosLocation;
	GLuint m_colorMapLocation;
	GLuint m_billboardSizeLocation;

public:

	PSBillboardTechnique();
	~PSBillboardTechnique();
	void SetVP(const Matrix4f& VP);
	void SetCameraPosition(const Vector3f& Pos);
	void SetColorTextureUnit(unsigned int TextureUnit);
	void SetBillboardSize(float BillboardSize);
};