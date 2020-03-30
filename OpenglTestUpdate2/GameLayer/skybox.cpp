#include "skybox.h"
#include "Pipeline.h"

SkyBox::SkyBox(const Camera* pCamera,
	const PersProjInfo& p)
{
	m_pCamera = pCamera;
	m_persProjInfo = p;

	m_pCubemapTex = NULL;
	m_pMesh = NULL;
}

SkyBox::~SkyBox()
{	
	if (m_pCubemapTex != nullptr)
	{
		delete m_pCubemapTex;
		m_pCubemapTex = nullptr;
	}
	if (m_pMesh != nullptr)
	{
		delete m_pMesh;
		m_pMesh = nullptr;
	}
}

bool SkyBox::Init(const string& Directory,
	const string& PosXFilename,
	const string& NegXFilename,
	const string& PosYFilename,
	const string& NegYFilename,
	const string& PosZFilename,
	const string& NegZFilename)
{
	std::cout << "test1" << std::endl;
	m_pCubemapTex = new CubemapTexture(Directory,
		PosXFilename,
		NegXFilename,
		PosYFilename,
		NegYFilename,
		PosZFilename,
		NegZFilename);
	//std::cout << "test2" << std::endl;
	if (!m_pCubemapTex->Load()) {
		return false;
	}
	//std::cout << "test3" << std::endl;
	m_pMesh = new Mesh();

	return m_pMesh->LoadMesh("Content/sphere.obj");
}

void SkyBox::Render()
{
	GLint OldCullFaceMode;
	glGetIntegerv(GL_CULL_FACE_MODE, &OldCullFaceMode);
	GLint OldDepthFuncMode;
	glGetIntegerv(GL_DEPTH_FUNC, &OldDepthFuncMode);

	glCullFace(GL_FRONT);
	glDepthFunc(GL_LEQUAL);

	/*Pipeline p;
	p.Scale(20.0f, 20.0f, 20.0f);
	p.Rotate(0.0f, 0.0f, 0.0f);
	p.WorldPos(m_pCamera->GetPos().x, m_pCamera->GetPos().y, m_pCamera->GetPos().z);
	p.SetCamera(m_pCamera->GetPos(), m_pCamera->GetTarget(), m_pCamera->GetUp());
	p.SetPerspectiveProj(m_persProjInfo);*/

	m_pCubemapTex->Bind(GL_TEXTURE0);
	m_pMesh->Render();

	glCullFace(OldCullFaceMode);
	glDepthFunc(OldDepthFuncMode);
}