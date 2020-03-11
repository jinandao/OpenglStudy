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

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

int length;
int length2;

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
Enemy* enemy = nullptr;
Enemy* enemy2 = nullptr;
Bullet* bullet = nullptr;

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
Vector3f quadScale(50, 0.1, 50);


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
	m_Vtransformation = GetWMatrixForCamera(pCamera->GetTarget(),pCamera->GetUp(),pCamera->GetPos());

	Matrix4f shadow_Vtransformation, shadow_VP, shadow_WVP;

	//绘制物体3
	//pTexture->Bind();

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
	
	m_Wtransformation = GetWMatrixForObject(Vector3f(10,10,10), quadRotate, pmesh1pos+Vector3f(5,5,3));
	m_WVPtransformation = m_VPtransformation * m_Wtransformation;
	glUniformMatrix4fv(gWorld, 1, GL_TRUE, (const GLfloat*)m_Wtransformation);
	glUniformMatrix4fv(gWVPLocation, 1, GL_TRUE, (const GLfloat*)m_WVPtransformation);
	pGeo2->Render();

	//初始化物体1相关矩阵
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

	//pMesh2->Render();
	player->Render();
	//绘制物体2
	
	m_Wtransformation = GetWMatrixForObject(enemy->GetScale(), enemy->GetRotation(), enemy->GetPos());
	m_WVPtransformation = m_VPtransformation * m_Wtransformation;
	glUniformMatrix4fv(gWorld, 1, GL_TRUE, (const GLfloat*)m_Wtransformation);
	glUniformMatrix4fv(gWVPLocation, 1, GL_TRUE, (const GLfloat*)m_WVPtransformation);
	enemy->Render();

	m_Wtransformation = GetWMatrixForObject(bullet->GetScale(), bullet->GetRotation(), bullet->GetPos());
	m_WVPtransformation = m_VPtransformation * m_Wtransformation;
	glUniformMatrix4fv(gWorld, 1, GL_TRUE, (const GLfloat*)m_Wtransformation);
	glUniformMatrix4fv(gWVPLocation, 1, GL_TRUE, (const GLfloat*)m_WVPtransformation);
	bullet->GetGeo()->Render();
	bullet->Move();

	m_Wtransformation = GetWMatrixForObject(enemy2->GetScale(), enemy2->GetRotation(), enemy2->GetPos());
	m_WVPtransformation = m_VPtransformation * m_Wtransformation;
	glUniformMatrix4fv(gWorld, 1, GL_TRUE, (const GLfloat*)m_Wtransformation);
	glUniformMatrix4fv(gWVPLocation, 1, GL_TRUE, (const GLfloat*)m_WVPtransformation);
	enemy2->Render();	

	//为物体2调整灯光矩阵
	//pShadowCamera->SetTarget(pmesh2pos);
	//初始化灯光相机相关矩阵	
	//shadow_Vtransformation = GetWMatrixForCamera(pShadowCamera->GetTarget(), pShadowCamera->GetUp(), pShadowCamera->GetPos());
	//shadow_VP = m_ProjTransformation * shadow_Vtransformation;
	//shadow_WVP = shadow_VP * m_Wtransformation;
	//glUniformMatrix4fv(gLightWVP, 1, GL_TRUE, (const GLfloat*)shadow_WVP);

	//pMesh->Render();

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
	std::cout << "Key:" << Key << std::endl;

	if (Key == 'q')
	{
		player->MoveForward();
		enemy->MoveBack();
	}
	if (Key == 'w')
	{
		player->MoveBack();
		enemy->MoveForward();
	}
	if (Key == 'e')
	{
		player->TurnLeft();
		enemy->TurnRight();
	}
	if (Key == 'r')
	{
		player->TurnRight();
		enemy->TurnLeft();
	}
	if (Key == 'a')
	{
		std::cout << "pos: " << player->GetPos().x << " " << player->GetPos().y << " " << player->GetPos().z << std::endl;
		std::cout << "rotate: " << player->GetRotation().x << " " << player->GetRotation().y << " " << player->GetRotation().z << std::endl;
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
	//pCamera = new Camera(WINDOW_WIDTH,WINDOW_HEIGHT);
	//pCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT,Vector3f(6,-13,23),Vector3f(0.4,-0.22,0.89),Vector3f(0,1,0));
	pCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT, Vector3f(0, -20, 50), Vector3f(0.5, -0.8, 0.2), Vector3f(0, 1, 0));
	pTexture = new Texture(0,"test.png");

	shadowMapFBO.Init(WINDOW_WIDTH, WINDOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);//!不bind不行！

	/*unsigned int Indices[] = { 0, 3, 1,
							1, 3, 2,
							2, 3, 0,
							1, 2, 0 };

	length2 = CreateIndexBuffer(&IBO, Indices, sizeof(Indices));
	length = CreateVertexBuffer(&VBO, Indices, 12);*/

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

	Vertex Vertices2[4] = { Vertex(Vector3f(0.0f, 0.0f, 0.0f), Vector2f(0.0f, 0.0f),Vector3f(0.0f,1.0f,0.0f)),
							   Vertex(Vector3f(0.0f, 1.0f, 1.0f), Vector2f(0.0f, 1.0f),Vector3f(0.0f,1.0f,0.0f)),
							   Vertex(Vector3f(1.0f, 0.0f, 0.0f), Vector2f(1.0f, 0.0f),Vector3f(0.0f,1.0f,0.0f)),
							   Vertex(Vector3f(1.0f, -1.0f, 1.0f), Vector2f(1.0f, 1.0f),Vector3f(0.0f,1.0f,0.0f)) };
	unsigned int Indices2[] = { 0, 3, 1,
							1, 3, 2};
	std::vector<Vertex> verts2(4);
	for (int i = 0; i < 4; i++)
	{
		verts[i] = Vertices[i];
	}
	std::vector<unsigned int> indices2(6);
	for (int i = 0; i < 6; i++)
	{
		indices[i] = Indices[i];
	}

	pGeo2 = new Geometry(1, 1);
	//pGeo2->AddMeshEntry(verts2, indices2, 0);
	pGeo2->AddMeshEntry(verts2, indices2, 0);
	std::string testpic2("Content/test3.png");
	pGeo2->AddTexure(3, testpic2);

	/*pMesh = new Mesh();
	pMesh->LoadMesh("Content/phoenix_ugv.md2");

	pMesh2 = new Mesh();
	pMesh2->LoadMesh("Content/phoenix_ugv.md2");*/

	player = new Player(Vector3f(0.1, 0.1, 0.1), Vector3f(0, 0, 0), Vector3f(25, 0, 25), 100, 20, 5, 5);
	player->SetMesh("Content/phoenix_ugv.md2");

	enemy = new Enemy(Vector3f(0.1, 0.1, 0.1), Vector3f(0, 0, 0), Vector3f(10, 0, 10), 50, 10, 3, 3);
	enemy->SetMesh("Content/phoenix_ugv.md2");

	enemy2 = new Enemy(Vector3f(0.1, 0.1, 0.1), Vector3f(0, 0, 0), Vector3f(30, 7, 12), 50, 10, 3, 3);
	enemy2->SetMesh(enemy->GetMesh());

	bullet = new Bullet(Vector3f(2,2,2),Vector3f(0,90,0),Vector3f(15,2,15),0.5);
	bullet->Init();

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	//length=CreateVertexBuffer(&VBO);
	//length2 = CreateIndexBuffer(&IBO);
	
	//std::cout << "length2" << length2 << std::endl;
	CompileLightShader();
	//CompileShadowShader();

	gPersProjInfo.FOV = 60.0f;
	gPersProjInfo.Height = WINDOW_HEIGHT;
	gPersProjInfo.Width = WINDOW_WIDTH;
	gPersProjInfo.zNear = 1.0f;
	gPersProjInfo.zFar = 100.0f;

	//初始化透视投影矩阵
	m_ProjTransformation.InitPersProjTransform(gPersProjInfo);

	glutMainLoop();

	delete player;

	return 0;
}