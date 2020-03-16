#include <glew.h>
#include <freeglut.h>
#include <iostream>
#include "OpenglTools.h"
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
#include "mesh.h"
#include "Lighting.h"
#include "MathTools.h"
#include "shadow_map_fbo.h"
#include "Game.h"
#include "GameLogic.h"
#include "Pipeline.h"

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

const char* pVSFileName = "lighting2.vs";
const char* pFSFileName = "lighting2.fs";

const char* pShadowVSFileName = "shadow_map.vs";
const char* pShadowFSFileName = "shadow_map.fs";

PersProjInfo gPersProjInfo;

ShadowMapFBO shadowMapFBO;

Texture* pTexture=nullptr;
Camera* pCamera = nullptr;

Geometry* pGeo = nullptr;
Geometry* pGeo2 = nullptr;

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

Vector3f pmesh1pos(25.0f, 0.0f, 25.0f);
Vector3f pmesh1rotate(0.0f, 0.0f, 0.0f);
Vector3f pmesh1scale(0.1, 0.1, 0.1);

Vector3f pmesh2pos(5.0f, 0.0f, 15.0f);
Vector3f pmesh2rotate(0.0f, 0.0f, 0.0f);
Vector3f pmesh2scale(0.1, 0.1, 0.1);

Vector3f quadPos(0, 0, 0);
Vector3f quadRotate(0, 0, 0);
Vector3f quadScale(100, 0.1, 100);

std::vector<Bullet*> bullets;
std::vector<std::pair<Enemy*,EnemyAI*>> enemies;

//Pipeline p;

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
	spotLight.Position = Vector3f(10, -17, 12);
	spotLight.Attenuation.Linear = 0.01f;
	spotLight.Direction = Vector3f(2.0f, -0.1f, 0.0f);
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
	glClear(GL_COLOR_BUFFER_BIT);

	Matrix4f shadowVtransformation = GetWMatrixForCamera(pShadowCamera->GetTarget(), pShadowCamera->GetUp(), pShadowCamera->GetPos());

	glUniformMatrix4fv(gShadowWVP, 1, GL_TRUE, (const GLfloat*)m_WVPtransformation);
	//player->Render();

	if (player != nullptr && player->isDead == false)
	{		
		m_Wtransformation = GetWMatrixForObject(player->GetScale(), player->GetRotation(), player->GetPos());
		m_VPtransformation = m_ProjTransformation * shadowVtransformation;
		m_WVPtransformation = m_VPtransformation * m_Wtransformation;
		glUniformMatrix4fv(gShadowWVP, 1, GL_TRUE, (const GLfloat*)m_WVPtransformation);
		
		player->Render();
	}

	for (auto ite = enemies.begin(); ite != enemies.end();)
	{
		if (ite->first->isDead == false)
		{			
			m_Wtransformation = GetWMatrixForObject(ite->first->GetScale(), ite->first->GetRotation(), ite->first->GetPos());
			m_VPtransformation = m_ProjTransformation * shadowVtransformation;
			m_WVPtransformation = m_VPtransformation * m_Wtransformation;
			glUniformMatrix4fv(gShadowWVP, 1, GL_TRUE, (const GLfloat*)m_WVPtransformation);

			ite->first->Render();			
		}
		ite++;
	}

	for (auto ite = bullets.begin(); ite != bullets.end();)
	{
		if ((*ite)->isEnd == false)
		{			
			m_Wtransformation = GetWMatrixForObject((*ite)->GetScale(), (*ite)->GetRotation(), (*ite)->GetPos());
			m_VPtransformation = m_ProjTransformation * shadowVtransformation;
			m_WVPtransformation = m_VPtransformation * m_Wtransformation;
			glUniformMatrix4fv(gShadowWVP, 1, GL_TRUE, (const GLfloat*)m_WVPtransformation);
			(*ite)->GetGeo()->Render();			
		}
		ite++;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

static void RenderPass()
{
	glUseProgram(ShaderProgram);

	//glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//pCamera->OnRender();

	SetLightsInShader();

	glUniform1i(gSampler, 0);
	glUniform1i(gShadowMap,1);

	glUniform3f(gEyeWorldPos, pCamera->GetPos().x, pCamera->GetPos().y, pCamera->GetPos().z);
	shadowMapFBO.BindForReading(1);
	

	//初始化相机相关矩阵
	if(player!=nullptr)
		pCamera->SetTarget(player->GetPos()+Vector3f(0,-9,0));

	//初始化灯光相机相关矩阵	
	Matrix4f shadow_Vtransformation, shadow_VP, shadow_WVP;
	shadow_Vtransformation = GetWMatrixForCamera(pShadowCamera->GetTarget(), pShadowCamera->GetUp(), pShadowCamera->GetPos());
	shadow_VP = m_ProjTransformation * shadow_Vtransformation;

	m_Vtransformation = GetWMatrixForCamera(pCamera->GetTarget(),pCamera->GetUp(),pCamera->GetPos());	

	//shadowMapFBO.BindForReading(0);
	pTexture->Bind();
	m_Wtransformation = GetWMatrixForObject(quadScale, quadRotate, quadPos);
	m_VPtransformation = m_ProjTransformation * m_Vtransformation;
	m_WVPtransformation = m_VPtransformation * m_Wtransformation;
	shadow_WVP = shadow_VP * m_Wtransformation;
	glUniformMatrix4fv(gWorld, 1, GL_TRUE, (const GLfloat*)m_Wtransformation);
	glUniformMatrix4fv(gWVPLocation, 1, GL_TRUE, (const GLfloat*)m_WVPtransformation);
	glUniformMatrix4fv(gLightWVP, 1, GL_TRUE, (const GLfloat*)shadow_WVP);
	pGeo->Render();

	//初始化物体1相关矩阵
	if (player!=nullptr&&player->isDead==false)
	{
		/*p.SetCamera(*pCamera);
		p.Scale(player->GetScale());
		p.Rotate(player->GetRotation());
		p.WorldPos(player->GetPos());
		glUniformMatrix4fv(gWorld, 1, GL_TRUE, (const GLfloat*)p.GetWorldTrans());
		glUniformMatrix4fv(gWVPLocation, 1, GL_TRUE, (const GLfloat*)p.GetWVPTrans());
		p.SetCamera(*pShadowCamera);
		glUniformMatrix4fv(gLightWVP, 1, GL_TRUE, (const GLfloat*)p.GetWVPTrans());
		player->Render();*/

		m_VPtransformation = m_ProjTransformation * m_Vtransformation;
		Matrix4f m_Wtransformation = GetWMatrixForObject(player->GetScale(), player->GetRotation(), player->GetPos());
		m_WVPtransformation = m_VPtransformation * m_Wtransformation;

		glUniformMatrix4fv(gWorld, 1, GL_TRUE, (const GLfloat*)m_Wtransformation);
		glUniformMatrix4fv(gWVPLocation, 1, GL_TRUE, (const GLfloat*)m_WVPtransformation);

		shadow_WVP = shadow_VP * m_Wtransformation;
		glUniformMatrix4fv(gLightWVP, 1, GL_TRUE, (const GLfloat*)shadow_WVP);
		player->Render();
	}
	else if(player!=nullptr)
	{		
		delete player;
		player = nullptr;
	}

	for (auto ite = enemies.begin(); ite != enemies.end();)
	{
		if(ite->first->isDead==false)
		{
			m_Wtransformation = GetWMatrixForObject(ite->first->GetScale(), ite->first->GetRotation(), ite->first->GetPos());
			m_WVPtransformation = m_VPtransformation * m_Wtransformation;
			glUniformMatrix4fv(gWorld, 1, GL_TRUE, (const GLfloat*)m_Wtransformation);
			glUniformMatrix4fv(gWVPLocation, 1, GL_TRUE, (const GLfloat*)m_WVPtransformation);

			shadow_WVP = shadow_VP * m_Wtransformation;
			glUniformMatrix4fv(gLightWVP, 1, GL_TRUE, (const GLfloat*)shadow_WVP);

			ite->first->Render();
			ite->second->Update(bullets);
			ite++;
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

	for (auto ite=bullets.begin();ite!=bullets.end();)
	{		
		if ((*ite)->isEnd==false)
		{
			m_Wtransformation = GetWMatrixForObject((*ite)->GetScale(), (*ite)->GetRotation(), (*ite)->GetPos());
			m_WVPtransformation = m_VPtransformation * m_Wtransformation;
			glUniformMatrix4fv(gWorld, 1, GL_TRUE, (const GLfloat*)m_Wtransformation);
			glUniformMatrix4fv(gWVPLocation, 1, GL_TRUE, (const GLfloat*)m_WVPtransformation);

			shadow_WVP = shadow_VP * m_Wtransformation;
			glUniformMatrix4fv(gLightWVP, 1, GL_TRUE, (const GLfloat*)shadow_WVP);

			(*ite)->GetGeo()->Render();
			(*ite)->Move();
			(*ite)->CheckHurt();
			ite++;
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
	glutSwapBuffers();
}

static void PassiveMouseCB(int x, int y)
{
	//pCamera->OnMouse(x, y);
}

static void KeyboardCB(unsigned char Key,int x,int y)
{	
	//std::cout << "Key:" << Key << std::endl;

	if (player != nullptr)
	{
		player->TakeAction(Key, bullets);
	}
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
	CompileShader(ShaderProgram, pVSFileName, pFSFileName);
	GetParamsInLightShader();
}

static void CompileShadowShader()
{
	CompileShader(ShadowShaderProgram, pShadowVSFileName, pShadowFSFileName);
	GetParamsInShadowShader();
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

	pCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT, Vector3f(0, -40, 50), Vector3f(0.5, -0.8, 0.2), Vector3f(0, 1, 0));
	//pCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT, Vector3f(0, 0, 0), Vector3f(0, 0, 1), Vector3f(0, 1, 0));
	pTexture = new Texture(0,"test.png");

	shadowMapFBO.Init(WINDOW_WIDTH, WINDOW_HEIGHT);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

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

	gPersProjInfo.FOV = 60.0f;
	gPersProjInfo.Height = WINDOW_HEIGHT;
	gPersProjInfo.Width = WINDOW_WIDTH;
	gPersProjInfo.zNear = 1.0f;
	gPersProjInfo.zFar = 500.0f;

	//初始化透视投影矩阵
	m_ProjTransformation.InitPersProjTransform(gPersProjInfo);

	glutMainLoop();

	return 0;
}

