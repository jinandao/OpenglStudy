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
PointLight pointLight;

GLuint gSampler;
GLuint gWVPLocation;
GLuint gShadowMap;

GLuint dirLightLocation_Color;
GLuint dirLightLocation_Ambient;
GLuint dirLightLocation_Diffuse;
GLuint dirLightLocation_Direction;

GLuint pointLightLocation_Color;
GLuint pointLightLocation_Ambient;
GLuint pointLightLocation_Diffuse;
GLuint pointLightLocation_Position;
GLuint pointLightLocation_Constant;
GLuint pointLightLocation_Linear;
GLuint pointLightLocation_Exp;

GLuint gLightWVP;//灯光WVP矩阵
GLuint gWorld;//物体世界矩阵
GLuint gEyeWorldPos;//相机位置
//GLuint gMatSpecularIntensity;
//GLuint gSpecularPower;

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

static void LightInit()
{
	directionLight.Color = COLOR_WHITE;
	directionLight.AmbientIntensity = 0.6;
	directionLight.DiffuseIntensity = 0.5;
	directionLight.Direction = Vector3f(0.5, 0.5, 0.5);

	pointLight.Color = COLOR_CYAN;
	pointLight.AmbientIntensity = 0.4;
	pointLight.DiffuseIntensity = 0.3;
	pointLight.Position = Vector3f(5, 8, 5);

	//阴影相机的位置
	//pShadowCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT, pointLight.Position, directionLight.Direction, Vector3f(0.0, 1.0, 0.0));
}

static void GetParamsInLightShader(GLuint ShaderProgram)
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

	GetParamsInShader(ShaderProgram, pointLightLocation_Color, "gPointLights.Base.Color");
	GetParamsInShader(ShaderProgram, pointLightLocation_Ambient, "gPointLights.Base.AmbientIntensity");
	GetParamsInShader(ShaderProgram, pointLightLocation_Diffuse, "gPointLights.Base.DiffuseIntensity");
	GetParamsInShader(ShaderProgram, pointLightLocation_Position, "gPointLights.Position");
	GetParamsInShader(ShaderProgram, pointLightLocation_Constant, "gPointLights.Atten.Constant");
	GetParamsInShader(ShaderProgram, pointLightLocation_Linear, "gPointLights.Atten.Linear");
	GetParamsInShader(ShaderProgram, pointLightLocation_Exp, "gPointLights.Atten.Exp");

	GetParamsInShader(ShaderProgram, gEyeWorldPos, "gEyeWorldPos");
}

static void GetParamsInShadowShader(GLuint ShaderProgram)
{
	gShadowWVP = glGetUniformLocation(ShadowShaderProgram, "gWVP");
	gShadowTex = glGetUniformLocation(ShadowShaderProgram, "gShadowMap");
}

static void SetLightsInShader()
{
	glUniform3f(dirLightLocation_Color, directionLight.Color.x, directionLight.Color.y, directionLight.Color.z);
	glUniform1f(dirLightLocation_Ambient, directionLight.AmbientIntensity);
	Vector3f Direction = directionLight.Direction;
	Direction.Normalize();
	glUniform3f(dirLightLocation_Direction, Direction.x, Direction.y, Direction.z);
	glUniform1f(dirLightLocation_Diffuse, directionLight.DiffuseIntensity);

	glUniform3f(pointLightLocation_Color, pointLight.Color.x, pointLight.Color.y, pointLight.Color.z);
	glUniform1f(pointLightLocation_Ambient, pointLight.AmbientIntensity);
	glUniform1f(pointLightLocation_Diffuse, pointLight.DiffuseIntensity);
	glUniform3f(pointLightLocation_Position, pointLight.Position.x, pointLight.Position.y, pointLight.Position.z);
	glUniform1f(pointLightLocation_Constant, pointLight.Attenuation.Constant);
	glUniform1f(pointLightLocation_Linear, pointLight.Attenuation.Linear);
	glUniform1f(pointLightLocation_Exp, pointLight.Attenuation.Exp);
}

//static void ShadowMapPass()
//{
//	glUseProgram(ShadowShaderProgram);
//
//	shadowMapFBO.BindForWriting();
//	glClear(GL_DEPTH_BUFFER_BIT);
//
//	//初始化透视投影矩阵
//	m_ProjTransformation.InitPersProjTransform(gPersProjInfo);
//
//	//物体1
//	//Vector3f pmesh1pos(-10.0f, 3.0f, 50.0f);
//	pShadowCamera->SetTarget(pmesh1pos);
//	//初始化相机相关矩阵
//	Matrix4f shadowVtransformation = GetWMatrixForCamera(pShadowCamera->GetTarget(), pShadowCamera->GetUp(), pShadowCamera->GetPos());
//	//初始化物体相关矩阵
//	Matrix4f ScaleTrans, RotateTrans, TranslationTrans;
//	m_Wtransformation = GetWMatrixForObject(Vector3f(0.2f, 0.2f, 0.2f), Vector3f(20, 20, 20), pmesh1pos);
//	m_VPtransformation = m_ProjTransformation * shadowVtransformation;
//	m_WVPtransformation = m_VPtransformation * m_Wtransformation;
//
//	glUniformMatrix4fv(gShadowWVP, 1, GL_TRUE, (const GLfloat*)m_WVPtransformation);
//	pMesh->Render();
//
//	//物体2
//	Vector3f pmesh2pos(1.0f, 5.5f, 4.0f);
//	pShadowCamera->SetTarget(pmesh1pos);
//	//初始化相机相关矩阵
//	shadowVtransformation = GetWMatrixForCamera(pShadowCamera->GetTarget(), pShadowCamera->GetUp(), pShadowCamera->GetPos());
//	//初始化物体相关矩阵	
//	m_Wtransformation = GetWMatrixForObject(Vector3f(0.2f, 0.2f, 0.2f), Vector3f(20, 20, 20), pmesh2pos);
//	m_VPtransformation = m_ProjTransformation * shadowVtransformation;
//	m_WVPtransformation = m_VPtransformation * m_Wtransformation;
//	glUniformMatrix4fv(gShadowWVP, 1, GL_TRUE, (const GLfloat*)m_WVPtransformation);
//	pMesh->Render();
//
//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
//}

static void RenderPass()
{
	glUseProgram(ShaderProgram);

	glClear(GL_COLOR_BUFFER_BIT);

	//pCamera->OnRender();

	SetLightsInShader();

	glUniform1i(gSampler, 0);
	glUniform1i(gShadowMap,1);

	glUniform3f(gEyeWorldPos, pCamera->GetPos().x, pCamera->GetPos().y, pCamera->GetPos().z);

	shadowMapFBO.BindForReading(1);

	//初始化相机相关矩阵
	if(player!=nullptr)
		pCamera->SetTarget(player->GetPos()+Vector3f(0,-9,0));

	m_Vtransformation = GetWMatrixForCamera(pCamera->GetTarget(),pCamera->GetUp(),pCamera->GetPos());

	Matrix4f shadow_Vtransformation, shadow_VP, shadow_WVP;

	//绘制物体3
	pTexture->Bind();
	m_Wtransformation = GetWMatrixForObject(quadScale, quadRotate, quadPos);
	m_WVPtransformation = m_VPtransformation * m_Wtransformation;
	glUniformMatrix4fv(gWorld, 1, GL_TRUE, (const GLfloat*)m_Wtransformation);
	glUniformMatrix4fv(gWVPLocation, 1, GL_TRUE, (const GLfloat*)m_WVPtransformation);

	//为物体3调整灯光矩阵
	//pShadowCamera->SetTarget(pmesh3pos);
	//初始化灯光相机相关矩阵	
	//shadow_Vtransformation = GetWMatrixForCamera(pShadowCamera->GetTarget(), pShadowCamera->GetUp(), pShadowCamera->GetPos());
	//shadow_VP = m_ProjTransformation * shadow_Vtransformation;
	//shadow_WVP = shadow_VP * m_Wtransformation;
	//glUniformMatrix4fv(gLightWVP, 1, GL_TRUE, (const GLfloat*)shadow_WVP);
	
	//DrawCall(VBO, IBO, length2);
	pGeo->Render();

	//初始化物体1相关矩阵
	if (player!=nullptr&&player->isDead==false)
	{
		m_VPtransformation = m_ProjTransformation * m_Vtransformation;
		Matrix4f m_Wtransformation = GetWMatrixForObject(player->GetScale(), player->GetRotation(), player->GetPos());
		m_WVPtransformation = m_VPtransformation * m_Wtransformation;
		glUniformMatrix4fv(gWorld, 1, GL_TRUE, (const GLfloat*)m_Wtransformation);
		glUniformMatrix4fv(gWVPLocation, 1, GL_TRUE, (const GLfloat*)m_WVPtransformation);
		//为物体1调整灯光矩阵
		//pShadowCamera->SetTarget(pmesh1pos);
		//初始化灯光相机相关矩阵	
		//shadow_Vtransformation = GetWMatrixForCamera(pShadowCamera->GetTarget(), pShadowCamera->GetUp(), pShadowCamera->GetPos());
		//shadow_VP = m_ProjTransformation * shadow_Vtransformation;
		//shadow_WVP = shadow_VP * m_Wtransformation;
		//glUniformMatrix4fv(gLightWVP, 1, GL_TRUE, (const GLfloat*)shadow_WVP);
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
	//ShadowMapPass();
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
		if (Key == 'x')
		{
			std::cout << "pos: " << player->GetPos().x << " " << player->GetPos().y << " " << player->GetPos().z << std::endl;
			std::cout << "rotate: " << player->GetRotation().x << " " << player->GetRotation().y << " " << player->GetRotation().z << std::endl;
			std::cout << "camera pos: " << pCamera->GetPos().x << " " << pCamera->GetPos().y << " " << pCamera->GetPos().z << std::endl;
			std::cout << "camera target: " << pCamera->GetTarget().x << " " << pCamera->GetTarget().y << " " << pCamera->GetTarget().z << std::endl;
		}
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
	GetParamsInLightShader(ShaderProgram);
}

static void CompileShadowShader()
{
	CompileShader(ShadowShaderProgram, pShadowVSFileName, pShadowFSFileName);
	GetParamsInShadowShader(ShadowShaderProgram);
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
	pGeo->AddMeshEntry(verts, indices,0);
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
	//CompileShadowShader();

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