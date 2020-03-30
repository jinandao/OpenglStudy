#include <glew.h>
#include <freeglut.h>
#include <iostream>
#include "EngineLayer/OpenglTools.h"
#include "EngineLayer/Shader.h"
#include "EngineLayer/Texture.h"
#include "GameLayer/Camera.h"
#include "EngineLayer/mesh.h"
#include "GameLayer/Lighting.h"
#include "EngineLayer/MathTools.h"
#include "EngineLayer/shadow_map_fbo.h"
#include "GameLayer/Game.h"

#include "GameLayer/Pipeline.h"
#include "GameLayer/skybox.h"

#include "GameLogic/Player.h"
#include "GameLogic/Enemy.h"
#include "GameLogic/EnemyAI.h"
#include "GameLogic/Bullet.h"

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

const char* pVSFileName = "Shaders/lighting2.vs";
const char* pFSFileName = "Shaders/lighting2.fs";

const char* pShadowVSFileName = "Shaders/shadow_map.vs";
const char* pShadowFSFileName = "Shaders/shadow_map.fs";

const char* skyboxVSShader = "Shaders/skybox.vs";
const char* skyboxFSShader = "Shaders/skybox.fs";

PersProjInfo gPersProjInfo;

ShadowMapFBO shadowMapFBO;

Texture* pTexture=nullptr;
Camera* pCamera = nullptr;

Geometry* pGeo = nullptr;
Geometry* pGeo2 = nullptr;
Geometry* pGeotest = nullptr;

Player* player = nullptr;

Camera* pShadowCamera = nullptr;

Matrix4f m_WVPtransformation;
Matrix4f m_VPtransformation;
Matrix4f m_WPtransformation;
Matrix4f m_WVtransformation;
Matrix4f m_Wtransformation;
Matrix4f m_Vtransformation;
Matrix4f m_ProjTransformation;

DirectionalLight directionLight;
//PointLight pointLight;
SpotLight spotLight;

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

GLuint gLightWVP;//灯光WVP矩阵
GLuint gWorld;//物体世界矩阵
GLuint gEyeWorldPos;//相机位置
GLuint gMatSpecularIntensity;
GLuint gSpecularPower;

GLuint gShadowWVP;
GLuint gShadowTex;

GLuint ShaderProgram;
GLuint ShadowShaderProgram;
GLuint SkyboxShaderProgram;

GLuint skyboxWVP;
GLuint skyboxTex;

Vector3f quadPos(0, 0, 0);
Vector3f quadRotate(0, 0, 0);
Vector3f quadScale(100, 0, 100);

std::vector<Bullet*> bullets;
std::vector<std::pair<Enemy*,EnemyAI*>> enemies;

Pipeline p;
SkyBox* skybox;

static void LightInit()
{
	directionLight.Color = COLOR_WHITE;
	directionLight.AmbientIntensity = 0.6;
	directionLight.DiffuseIntensity = 0.5;
	directionLight.Direction = Vector3f(0.5, 0.5, 0.5);

	/*pointLight.Color = COLOR_CYAN;
	pointLight.AmbientIntensity = 0.4;
	pointLight.DiffuseIntensity = 0.3;
	pointLight.Position = Vector3f(5, 8, 5);*/

	spotLight.Color = COLOR_CYAN;
	spotLight.AmbientIntensity = 0.1;
	spotLight.DiffuseIntensity = 0.8;
	spotLight.Position = Vector3f(5, 20, 5);
	spotLight.Attenuation.Linear = 0.01f;
	spotLight.Direction = Vector3f(0.5f, -0.5f, 0);
	spotLight.Cutoff = 0.0f;

	//阴影相机的位置
	pShadowCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT, spotLight.Position, spotLight.Direction, Vector3f(0.0, 1.0, 0.0));
}

static void GetParamsInLightShader()
{
	GetParamsInShader(ShaderProgram, gSampler, "gSampler");
	GetParamsInShader(ShaderProgram, gWVPLocation, "gWVP");
	GetParamsInShader(ShaderProgram, gWorld, "gWorld");
	GetParamsInShader(ShaderProgram, gLightWVP, "gLightWVP");
	GetParamsInShader(ShaderProgram, gShadowMap, "gShadowMap");

	GetParamsInShader(ShaderProgram, dirLightLocation_Color, "gDirectionalLight.Base.Color");
	GetParamsInShader(ShaderProgram, dirLightLocation_Ambient, "gDirectionalLight.Base.AmbientIntensity");
	GetParamsInShader(ShaderProgram, dirLightLocation_Diffuse, "gDirectionalLight.Base.DiffuseIntensity");
	GetParamsInShader(ShaderProgram, dirLightLocation_Direction, "gDirectionalLight.Direction");

	GetParamsInShader(ShaderProgram, spotLightLocation_Color, "gSpotLights.Base.Base.Color");
	GetParamsInShader(ShaderProgram, spotLightLocation_Ambient, "gSpotLights.Base.Base.AmbientIntensity");
	GetParamsInShader(ShaderProgram, spotLightLocation_Diffuse, "gSpotLights.Base.Base.DiffuseIntensity");
	GetParamsInShader(ShaderProgram, spotLightLocation_Position, "gSpotLights.Base.Position");
	GetParamsInShader(ShaderProgram, spotLightLocation_Constant, "gSpotLights.Base.Atten.Constant");
	GetParamsInShader(ShaderProgram, spotLightLocation_Linear, "gSpotLights.Base.Atten.Linear");
	GetParamsInShader(ShaderProgram, spotLightLocation_Exp, "gSpotLights.Base.Atten.Exp");
	GetParamsInShader(ShaderProgram, spotLightLocation_Direction, "gSpotLights.Direction");
	GetParamsInShader(ShaderProgram, spotLightLocation_Cutoff, "gSpotLights.Cutoff");

	GetParamsInShader(ShaderProgram, gEyeWorldPos, "gEyeWorldPos");
	GetParamsInShader(ShaderProgram, gMatSpecularIntensity, "gMatSpecularIntensity");
	GetParamsInShader(ShaderProgram, gSpecularPower, "gSpecularPower");
}

static void GetParamsInShadowShader()
{
	GetParamsInShader(ShadowShaderProgram, gShadowWVP, "gWVP");
	GetParamsInShader(ShadowShaderProgram, gShadowTex, "gShadowMap");
}

static void GetParamsInSkyboxShader()
{
	GetParamsInShader(SkyboxShaderProgram, skyboxWVP, "gWVP");
	GetParamsInShader(SkyboxShaderProgram, skyboxTex, "gCubemapTexture");
	if (skyboxWVP == 0xFFFFFFFF)
	{
		std::cout << "skyboxwvpnotfound" << std::endl;
	}
	if (skyboxTex == 0xFFFFFFFF)
	{
		std::cout << "skyboxtexnotfound" << std::endl;
	}
}


static void SetLightsInShader()
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

static void ShadowMapPass()
{
	glUseProgram(ShadowShaderProgram);

	shadowMapFBO.BindForWriting();
	
	//glClear(GL_DEPTH_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	p.SetCamera(*pShadowCamera);
	if (player != nullptr && player->isDead == false)
	{				
		p.Scale(player->GetScale());
		p.WorldPos(player->GetPos());
		p.Rotate(player->GetRotation());
		glUniformMatrix4fv(gShadowWVP, 1, GL_TRUE, (const GLfloat*)p.GetWVPTrans());
		
		player->Render();
	}

	for (auto ite = enemies.begin(); ite != enemies.end();)
	{
		if (ite->first->isDead == false)
		{			
			
			p.Scale(ite->first->GetScale());
			p.WorldPos(ite->first->GetPos());
			p.Rotate(ite->first->GetRotation());
			glUniformMatrix4fv(gShadowWVP, 1, GL_TRUE, (const GLfloat*)p.GetWVPTrans());

			ite->first->Render();			
		}
		ite++;
	}

	for (auto ite = bullets.begin(); ite != bullets.end();)
	{
		if ((*ite)->isEnd == false)
		{			
			
			p.Scale((*ite)->GetScale());
			p.WorldPos((*ite)->GetPos());
			p.Rotate((*ite)->GetRotation());
			glUniformMatrix4fv(gShadowWVP, 1, GL_TRUE, (const GLfloat*)p.GetWVPTrans());
			(*ite)->GetGeo()->Render();			
		}
		ite++;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

static void RenderPass()
{
	pCamera->OnRender();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	p.SetCamera(*pCamera);
	p.SetPerspectiveProj(gPersProjInfo);
	
	glUseProgram(SkyboxShaderProgram);
	Pipeline p;
	p.Scale(20.0f, 20.0f, 20.0f);
	p.Rotate(0.0f, 0.0f, 0.0f);
	p.WorldPos(pCamera->GetPos().x, pCamera->GetPos().y, pCamera->GetPos().z);
	p.SetCamera(pCamera->GetPos(), pCamera->GetTarget(), pCamera->GetUp());
	p.SetPerspectiveProj(gPersProjInfo);
	glUniformMatrix4fv(skyboxWVP, 1, GL_TRUE, (const GLfloat*)p.GetWVPTrans());
	glUniform1i(skyboxTex, 0);
	skybox->Render();

	glUseProgram(ShadowShaderProgram);	
	p.Scale(quadScale);
	Vector3f test = quadPos + Vector3f(0, 17, 0);
	p.WorldPos(test);
	p.Rotate(quadRotate);
	glUniformMatrix4fv(gShadowWVP, 1, GL_TRUE, (const GLfloat*)p.GetWVPTrans());
	shadowMapFBO.BindForReading(0);
	pGeo->Render();

	glUseProgram(ShaderProgram);
	SetLightsInShader();
	glUniform1i(gSampler, 0);
	glUniform1i(gShadowMap, 1);
	shadowMapFBO.BindForReading(1);

	p.Scale(quadScale);
	p.WorldPos(quadPos);
	p.Rotate(quadRotate);
	glUniformMatrix4fv(gWorld, 1, GL_TRUE, (const GLfloat*)p.GetWorldTrans());
	glUniformMatrix4fv(gWVPLocation, 1, GL_TRUE, (const GLfloat*)p.GetWVPTrans());
	p.SetCamera(*pShadowCamera);
	glUniformMatrix4fv(gLightWVP, 1, GL_TRUE, (const GLfloat*)p.GetWVPTrans());
	pTexture->Bind();
	//shadowMapFBO.BindForReading(0);
	pGeo->Render();

	p.SetCamera(*pCamera);
	if (player != nullptr && player->isDead == false)
	{
		p.Scale(player->GetScale());
		p.WorldPos(player->GetPos());
		p.Rotate(player->GetRotation());

		glUniformMatrix4fv(gWorld, 1, GL_TRUE, (const GLfloat*)p.GetWorldTrans());
		glUniformMatrix4fv(gWVPLocation, 1, GL_TRUE, (const GLfloat*)p.GetWVPTrans());

		p.SetCamera(*pShadowCamera);
		glUniformMatrix4fv(gLightWVP, 1, GL_TRUE, (const GLfloat*)p.GetWVPTrans());
		player->Render();
		p.SetCamera(*pCamera);
	}
	else if(player!=nullptr)
	{
		delete player;
		player = nullptr;
	}

	for (auto ite = enemies.begin(); ite != enemies.end();)
	{
		if (ite->first->isDead == false)
		{
			p.Scale(ite->first->GetScale());
			p.WorldPos(ite->first->GetPos());
			p.Rotate(ite->first->GetRotation());
			glUniformMatrix4fv(gWorld, 1, GL_TRUE, (const GLfloat*)p.GetWorldTrans());
			glUniformMatrix4fv(gWVPLocation, 1, GL_TRUE, (const GLfloat*)p.GetWVPTrans());
			
			p.SetCamera(*pShadowCamera);
			glUniformMatrix4fv(gLightWVP, 1, GL_TRUE, (const GLfloat*)p.GetWVPTrans());

			ite->first->Render();
			ite->second->Update(bullets);
			ite++;
			p.SetCamera(*pCamera);
		}
		else
		{
			auto toDel1 = ite->first;
			auto toDel2 = ite->second;
			ite = enemies.erase(ite);
			Destroy(toDel1);
			delete toDel2;
		}
	}

	for (auto ite = bullets.begin(); ite != bullets.end();)
	{
		if ((*ite)->isEnd == false)
		{
			m_Wtransformation = GetWMatrixForObject((*ite)->GetScale(), (*ite)->GetRotation(), (*ite)->GetPos());
			m_WVPtransformation = m_VPtransformation * m_Wtransformation;
			
			p.Scale((*ite)->GetScale());
			p.WorldPos((*ite)->GetPos());
			p.Rotate((*ite)->GetRotation());
			glUniformMatrix4fv(gWorld, 1, GL_TRUE, (const GLfloat*)p.GetWorldTrans());
			glUniformMatrix4fv(gWVPLocation, 1, GL_TRUE, (const GLfloat*)p.GetWVPTrans());
			
			p.SetCamera(*pShadowCamera);
			glUniformMatrix4fv(gLightWVP, 1, GL_TRUE, (const GLfloat*)p.GetWVPTrans());

			(*ite)->GetGeo()->Render();
			(*ite)->Move();
			(*ite)->CheckHurt();
			ite++;

			p.SetCamera(*pCamera);
		}
		else
		{
			auto toDel = *ite;
			ite = bullets.erase(ite);
			Destroy(toDel);
		}
	}
}

static void RenderSceneCB()
{
	ShadowMapPass();
	RenderPass();
	
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glUniform1i(gShadowMap,0);
	//pCamera->OnRender();
	//pCamera->SetPos(Vector3f(0, 0, 0));
	//pCamera->SetTarget(Vector3f(0, 0, 1));
	//p.SetCamera(*pCamera);
	//p.SetPerspectiveProj(gPersProjInfo);
	//p.Scale(Vector3f(5,5,5));
	//p.WorldPos(Vector3f(-3,3,5));
	//p.Rotate(Vector3f(90,0,0));
	//glUniformMatrix4fv(gShadowWVP, 1, GL_TRUE, (const GLfloat*)p.GetWVPTrans());
	////pTexture->Bind();	
	//shadowMapFBO.BindForReading(0);
	//pGeo->Render();
	////pGeotest->Render();
	////player->Render();

	glutSwapBuffers();
}

static void PassiveMouseCB(int x, int y)
{
	pCamera->OnMouse(x, y);
}

static void KeyboardCB(unsigned char Key,int x,int y)
{	
	//std::cout << "Key:" << Key << std::endl;

	if (player != nullptr)
	{
		player->TakeAction(Key, bullets);
	}
	/*if (Key == 'z')
	{
		Vector3f temp = pCamera->GetPos();
		temp.x += 0.1;
		pCamera->SetPos(temp);
	}
	if (Key == 'x')
	{
		Vector3f temp = pCamera->GetPos();
		temp.y += 0.1;
		pCamera->SetPos(temp);
	}
	if (Key == 'c')
	{
		Vector3f temp = pCamera->GetPos();
		temp.z += 0.1;
		pCamera->SetPos(temp);
	}
	if (Key == 'v')
	{
		Vector3f temp = pCamera->GetPos();
		temp.x -= 0.1;
		pCamera->SetPos(temp);
	}
	if (Key == 'b')
	{
		Vector3f temp = pCamera->GetPos();
		temp.y -= 0.1;
		pCamera->SetPos(temp);
	}
	if (Key == 'n')
	{
		Vector3f temp = pCamera->GetPos();
		temp.z -= 0.1;
		pCamera->SetPos(temp);
	}
	if (Key == 'm')
	{
		Vector3f temp = pCamera->GetPos();
		Vector3f temp2 = pCamera->GetTarget();
		std::cout << "camera pos:" << temp.x<<" "<<temp.y<<" "<<temp.z<<std::endl;
		std::cout << "camera target:" << temp2.x << " " << temp2.y << " " << temp2.z << std::endl;
	}*/
}

static void InitializeGlutCallbacks()
{
	glutDisplayFunc(RenderSceneCB);
	glutIdleFunc(RenderSceneCB);
	glutPassiveMotionFunc(PassiveMouseCB);
	glutKeyboardFunc(KeyboardCB);
}

static void CompileLightShader()
{
	//CompileShader(ShaderProgram, pVSFileName, pFSFileName);
	Shader shader(pVSFileName, pFSFileName);
	if (shader.Init())
	{
		ShaderProgram = shader.ShaderID();
	}
	
	GetParamsInLightShader();
}

static void CompileShadowShader()
{
	//CompileShader(ShadowShaderProgram, pShadowVSFileName, pShadowFSFileName);
	Shader shadowshader(pShadowVSFileName, pShadowFSFileName);
	if (shadowshader.Init())
	{
		ShadowShaderProgram = shadowshader.ShaderID();
	}
	GetParamsInShadowShader();
}

static void CompileSkyboxShader()
{
	Shader skyboxShader(skyboxVSShader,skyboxFSShader);
	if (skyboxShader.Init())
	{
		SkyboxShaderProgram = skyboxShader.ShaderID();
	}
	GetParamsInSkyboxShader();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(1024, 768);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Tutorial 01");

	//glutEnterGameMode();//?

	InitializeGlutCallbacks();

	GLenum res = glewInit();
	if (res != GLEW_OK) {
		return 1;
	}

	LightInit();

	//pCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT, Vector3f(0, -40, 50), Vector3f(0.5, -0.8, 0.2), Vector3f(0, 1, 0));
	pCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT, Vector3f(20, 30, -20), Vector3f(0, -0.3, 0.9), Vector3f(0, 1, 0));
	pTexture = new Texture(0,"test.png");	

	shadowMapFBO.Init(WINDOW_WIDTH, WINDOW_HEIGHT);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	Vertex Verticestest[3] = { Vertex(Vector3f(-1.0f, -1.0f, 0.0f), Vector2f(0.0f, 0.0f),Vector3f(0.0f,1.0f,0.0f)),
							 Vertex(Vector3f(1.0f, -1.0f, 1.0f), Vector2f(0.0f, 1.0f),Vector3f(0.0f,1.0f,0.0f)),
							Vertex(Vector3f(0.0f, 1.0f, 0.0f), Vector2f(1.0f, 0.0f),Vector3f(0.0f,1.0f,0.0f)) };
	unsigned int Indicestest[3] = { 0,1,2 };
	std::vector<Vertex> vertstest(4);
	for (int i = 0; i < 3; i++)
	{
		vertstest[i] = Verticestest[i];
	}
	std::vector<unsigned int> indicestest(3);
	for (int i = 0; i < 3; i++)
	{
		indicestest[i] = Indicestest[i];
	}
	pGeotest = new Geometry(1, 1);
	pGeotest->AddMeshEntry(vertstest, indicestest, 2);
	std::string testpictest("test.png");
	pGeotest->AddTexure(0, testpictest);


	Vertex Vertices[4] = { Vertex(Vector3f(0.0f, 0.0f, 0.0f), Vector2f(0.0f, 0.0f),Vector3f(0.0f,1.0f,0.0f)),
							   Vertex(Vector3f(0.0f, 0.0f, 1.0f), Vector2f(0.0f, 1.0f),Vector3f(0.0f,1.0f,0.0f)),
							   Vertex(Vector3f(1.0f, 0.0f, 0.0f), Vector2f(1.0f, 0.0f),Vector3f(0.0f,1.0f,0.0f)),
							   Vertex(Vector3f(1.0f, 0.0f, 1.0f), Vector2f(1.0f, 1.0f),Vector3f(0.0f,1.0f,0.0f)) };
	unsigned int Indices[] = { 0, 3, 1,
							1, 3, 2,
							2, 3, 0,
							1, 2, 0 };
	std::vector<Vertex> verts(4);
	for (int i = 0; i < 4; i++)
	{
		verts[i] = Vertices[i];
	}
	std::vector<unsigned int> indices(12);
	for (int i = 0; i < 12; i++)
	{
		indices[i] = Indices[i];
	}

	pGeo=new Geometry(1,1);
	pGeo->AddMeshEntry(verts, indices,2);
	std::string testpic("test.png");
	pGeo->AddTexure(0, testpic);

	player = new Player(Vector3f(0.1, 0.1, 0.1), Vector3f(0, 0, 0), Vector3f(25, 0, 25), 100, 1.5, 5);
	player->SetMesh("Content/phoenix_ugv.md2");

	Enemy* enemy = new Enemy(Vector3f(0.1, 0.1, 0.1), Vector3f(0, 0, 0), Vector3f(10, 0, 10), 50, 1, 3);
	enemy->SetMesh("Content/phoenix_ugv.md2");
	EnemyAI* enemyAI = new EnemyAI(player, enemy);
	Enemy* enemy2 = new Enemy(Vector3f(0.1, 0.1, 0.1), Vector3f(0, 0, 0), Vector3f(30, 0, 12), 50, 1, 3);
	enemy2->SetMesh(enemy->GetMesh());
	EnemyAI* enemyAI2 = new EnemyAI(player, enemy2);
	Enemy* enemy3 = new Enemy(Vector3f(0.1, 0.1, 0.1), Vector3f(0, 20, 0), Vector3f(40, 0, 43), 50, 1, 3);
	enemy3->SetMesh(enemy->GetMesh());
	EnemyAI* enemyAI3 = new EnemyAI(player, enemy3);
	Enemy* enemy4 = new Enemy(Vector3f(0.1, 0.1, 0.1), Vector3f(0, 30, 0), Vector3f(45, 0, 19), 50, 1, 3);
	enemy4->SetMesh(enemy->GetMesh());
	EnemyAI* enemyAI4 = new EnemyAI(player, enemy4);

	enemies.emplace_back(std::make_pair(enemy,enemyAI));
	enemies.emplace_back(std::make_pair(enemy2, enemyAI2));
	enemies.emplace_back(std::make_pair(enemy3, enemyAI3));
	enemies.emplace_back(std::make_pair(enemy4, enemyAI4));

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	CompileLightShader();
	CompileShadowShader();
	CompileSkyboxShader();

	gPersProjInfo.FOV = 60.0f;
	gPersProjInfo.Height = WINDOW_HEIGHT;
	gPersProjInfo.Width = WINDOW_WIDTH;
	gPersProjInfo.zNear = 1.0f;
	gPersProjInfo.zFar = 500.0f;

	skybox = new SkyBox(pCamera, gPersProjInfo);

	skybox->Init("","Content/sp3right.jpg",
		"Content/sp3left.jpg",
		"Content/sp3top.jpg",
		"Content/sp3bot.jpg",
		"Content/sp3front.jpg",
		"Content/sp3back.jpg");

	//初始化透视投影矩阵
	m_ProjTransformation.InitPersProjTransform(gPersProjInfo);
	
	glutMainLoop();

	return 0;
}

