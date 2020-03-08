#include <glew.h>
#include <freeglut.h>
#include <iostream>
#include "OpenglTools.h"
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
#include "mesh.h"
#include "Lighting.h"
#include "shadow_map_fbo.h"

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

GLuint VBO;
GLuint IBO;

int length;
int length2;

const char* pVSFileName = "lighting2.vs";
const char* pFSFileName = "lighting2.fs";

const char* pShadowVSFileName = "shadow_map.vs";
const char* pShadowFSFileName = "shadow_map.fs";

//GLuint gScaleLocation;//对shader参数的调整需要怎么封装？
PersProjInfo gPersProjInfo;

ShadowMapFBO shadowMapFBO;

Texture* pTexture=nullptr;
Camera* pCamera = nullptr;
Mesh* pMesh = nullptr;
Mesh* pMesh2 = nullptr;

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

struct RenderItem
{
	Vector3f scale;
	Vector3f worldPos;
	Vector3f rotateInfo;
};

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
	pShadowCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT, pointLight.Position, directionLight.Direction, Vector3f(0.0, 1.0, 0.0));
}

static void GetParamsInShader(GLuint ShaderProgram)
{
	gSampler = glGetUniformLocation(ShaderProgram, "gSampler");
	gWVPLocation = glGetUniformLocation(ShaderProgram, "gWVP");
	gWorld= glGetUniformLocation(ShaderProgram, "gWorld");
	gLightWVP = glGetUniformLocation(ShaderProgram, "gLightWVP");
	gShadowMap= glGetUniformLocation(ShaderProgram, "gShadowMap");

	dirLightLocation_Color= glGetUniformLocation(ShaderProgram,"gDirectionalLight.Base.Color");
	dirLightLocation_Ambient = glGetUniformLocation(ShaderProgram, "gDirectionalLight.Base.AmbientIntensity");
	dirLightLocation_Diffuse= glGetUniformLocation(ShaderProgram, "gDirectionalLight.Base.DiffuseIntensity");
	dirLightLocation_Direction= glGetUniformLocation(ShaderProgram, "gDirectionalLight.Direction");

	pointLightLocation_Color = glGetUniformLocation(ShaderProgram, "gPointLights.Base.Color");;//未初始化
	pointLightLocation_Ambient = glGetUniformLocation(ShaderProgram, "gPointLights.Base.AmbientIntensity");;
	pointLightLocation_Diffuse = glGetUniformLocation(ShaderProgram, "gPointLights.Base.DiffuseIntensity");;
	pointLightLocation_Position = glGetUniformLocation(ShaderProgram, "gPointLights.Position");;
	pointLightLocation_Constant = glGetUniformLocation(ShaderProgram, "gPointLights.Atten.Constant");;
	pointLightLocation_Linear = glGetUniformLocation(ShaderProgram, "gPointLights.Atten.Linear");;
	pointLightLocation_Exp = glGetUniformLocation(ShaderProgram, "gPointLights.Atten.Exp");;

	//gMatSpecularIntensity = glGetUniformLocation(ShaderProgram, "gMatSpecularIntensity");
	//gSpecularPower = glGetUniformLocation(ShaderProgram, "gSpecularPower");
	gEyeWorldPos = glGetUniformLocation(ShaderProgram, "gEyeWorldPos");

	if (gSampler == 0xFFFFFFFF)
	{
		std::cout << "gSampler not found" << std::endl;
	}
	if (gWVPLocation == 0xFFFFFFFF)
	{
		std::cout << "gWVPLocation not found" << std::endl;
	}
	if (gWorld == 0xFFFFFFFF)
	{
		std::cout << "gWorld not found" << std::endl;
	}
	if (gLightWVP == 0xFFFFFFFF)
	{
		std::cout << "gLightWVP not found" << std::endl;
	}
	if (gShadowMap == 0xFFFFFFFF)
	{
		std::cout << "gShadowMap not found" << std::endl;
	}
	if (dirLightLocation_Color == 0xFFFFFFFF)
	{
		std::cout << "dirLightLocation_Color not found" << std::endl;
	}
	if (dirLightLocation_Ambient == 0xFFFFFFFF)
	{
		std::cout << "dirLightLocation_Ambient not found" << std::endl;
	}
	if (dirLightLocation_Diffuse == 0xFFFFFFFF)
	{
		std::cout << "dirLightLocation_Diffuse not found" << std::endl;
	}
	if (dirLightLocation_Direction == 0xFFFFFFFF)
	{
		std::cout << "dirLightLocation_Direction not found" << std::endl;
	}
	if (pointLightLocation_Color == 0xFFFFFFFF)
	{
		std::cout << "pointLightLocation_Color not found" << std::endl;
	}
	if (pointLightLocation_Ambient == 0xFFFFFFFF)
	{
		std::cout << "pointLightLocation_Ambient not found" << std::endl;
	}
	if (pointLightLocation_Diffuse == 0xFFFFFFFF)
	{
		std::cout << "pointLightLocation_Diffuse not found" << std::endl;
	}
	if (pointLightLocation_Position == 0xFFFFFFFF)
	{
		std::cout << "pointLightLocation_Position not found" << std::endl;
	}
	if (pointLightLocation_Constant == 0xFFFFFFFF)
	{
		std::cout << "pointLightLocation_Constant not found" << std::endl;
	}
	if (pointLightLocation_Linear == 0xFFFFFFFF)
	{
		std::cout << "pointLightLocation_Linear not found" << std::endl;
	}
	if (pointLightLocation_Exp == 0xFFFFFFFF)
	{
		std::cout << "pointLightLocation_Exp not found" << std::endl;
	}
	if(gEyeWorldPos==0xFFFFFFFF)
	{
		std::cout << "gEyeWorldPos not found" << std::endl;
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

	glUniform3f(pointLightLocation_Color, pointLight.Color.x, pointLight.Color.y, pointLight.Color.z);
	glUniform1f(pointLightLocation_Ambient, pointLight.AmbientIntensity);
	glUniform1f(pointLightLocation_Diffuse, pointLight.DiffuseIntensity);
	glUniform3f(pointLightLocation_Position, pointLight.Position.x, pointLight.Position.y, pointLight.Position.z);
	glUniform1f(pointLightLocation_Constant, pointLight.Attenuation.Constant);
	glUniform1f(pointLightLocation_Linear, pointLight.Attenuation.Linear);
	glUniform1f(pointLightLocation_Exp, pointLight.Attenuation.Exp);
}

static void ShadowMapPass()
{
	glUseProgram(ShadowShaderProgram);

	shadowMapFBO.BindForWriting();
	glClear(GL_DEPTH_BUFFER_BIT);

	//初始化透视投影矩阵
	m_ProjTransformation.InitPersProjTransform(gPersProjInfo);

	//物体1
	Vector3f pmesh1pos(-10.0f, 3.0f, 50.0f);
	pShadowCamera->SetTarget(pmesh1pos);
	//初始化相机相关矩阵
	Matrix4f ShadowCameraTranslationTrans, ShadowCameraRotateTrans, shadowVtransformation;
	ShadowCameraTranslationTrans.InitTranslationTransform(-pShadowCamera->GetPos().x, -pShadowCamera->GetPos().y, -pShadowCamera->GetPos().z);
	ShadowCameraRotateTrans.InitCameraTransform(pShadowCamera->GetTarget(), pShadowCamera->GetUp());
	shadowVtransformation = ShadowCameraTranslationTrans * ShadowCameraRotateTrans;
	//初始化物体相关矩阵
	Matrix4f ScaleTrans, RotateTrans, TranslationTrans;
	ScaleTrans.InitScaleTransform(1.0f, 1.0f, 1.0f);
	RotateTrans.InitRotateTransform(0.0f, 0.0f, 0.0f);
	TranslationTrans.InitTranslationTransform(pmesh1pos.x, pmesh1pos.y, pmesh1pos.z);
	m_Wtransformation = TranslationTrans * RotateTrans * ScaleTrans;
	m_VPtransformation = m_ProjTransformation * shadowVtransformation;
	m_WVPtransformation = m_VPtransformation * m_Wtransformation;
	glUniformMatrix4fv(gWVPLocation, 1, GL_TRUE, (const GLfloat*)m_WVPtransformation);
	pMesh->Render();

	//物体2
	Vector3f pmesh2pos(1.0f, 5.5f, 4.0f);
	pShadowCamera->SetTarget(pmesh1pos);
	//初始化相机相关矩阵
	ShadowCameraTranslationTrans.InitTranslationTransform(-pShadowCamera->GetPos().x, -pShadowCamera->GetPos().y, -pShadowCamera->GetPos().z);
	ShadowCameraRotateTrans.InitCameraTransform(pShadowCamera->GetTarget(), pShadowCamera->GetUp());
	shadowVtransformation = ShadowCameraTranslationTrans * ShadowCameraRotateTrans;
	//初始化物体相关矩阵
	ScaleTrans.InitScaleTransform(1.0f, 1.0f, 1.0f);
	RotateTrans.InitRotateTransform(0.0f, 0.0f, 0.0f);
	TranslationTrans.InitTranslationTransform(pmesh2pos.x, pmesh2pos.y, pmesh2pos.z);
	m_Wtransformation = TranslationTrans * RotateTrans * ScaleTrans;
	m_VPtransformation = m_ProjTransformation * shadowVtransformation;
	m_WVPtransformation = m_VPtransformation * m_Wtransformation;
	glUniformMatrix4fv(gWVPLocation, 1, GL_TRUE, (const GLfloat*)m_WVPtransformation);
	pMesh->Render();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

static void RenderPass()
{
	glUseProgram(ShaderProgram);

	glClear(GL_COLOR_BUFFER_BIT);

	pCamera->OnRender();

	SetLightsInShader();

	glUniform1i(gSampler, 0);
	glUniform1i(gShadowMap,1);

	glUniform3f(gEyeWorldPos, pCamera->GetPos().x, pCamera->GetPos().y, pCamera->GetPos().z);

	shadowMapFBO.BindForReading(1);

	//初始化相机相关矩阵
	Matrix4f CameraTranslationTrans, CameraRotateTrans;
	CameraTranslationTrans.InitTranslationTransform(-pCamera->GetPos().x, -pCamera->GetPos().y, -pCamera->GetPos().z);
	CameraRotateTrans.InitCameraTransform(pCamera->GetTarget(), pCamera->GetUp());
	m_Vtransformation = CameraRotateTrans * CameraTranslationTrans;

	Vector3f pmesh1pos(-10.0f, 3.0f, 50.0f);
	//初始化物体相关矩阵
	Matrix4f ScaleTrans, RotateTrans, TranslationTrans;
	ScaleTrans.InitScaleTransform(0.2f, 0.2f, 0.2f);
	RotateTrans.InitRotateTransform(30.0f, 30.0f, 30.0f);
	TranslationTrans.InitTranslationTransform(pmesh1pos.x, pmesh1pos.y, pmesh1pos.z);
	m_Wtransformation = TranslationTrans * RotateTrans * ScaleTrans;
	m_VPtransformation = m_ProjTransformation * m_Vtransformation;
	m_WVPtransformation = m_VPtransformation * m_Wtransformation;
	glUniformMatrix4fv(gWorld, 1, GL_TRUE, (const GLfloat*)m_Wtransformation);
	glUniformMatrix4fv(gWVPLocation, 1, GL_TRUE, (const GLfloat*)m_WVPtransformation);

	//为物体1调整灯光矩阵
	pShadowCamera->SetTarget(pmesh1pos);
	//初始化灯光相机相关矩阵
	Matrix4f shadowCameraTranslationTrans, shadowCameraRotateTrans, shadow_Vtransformation, shadow_VP, shadow_WVP;
	shadowCameraTranslationTrans.InitTranslationTransform(-pShadowCamera->GetPos().x, -pShadowCamera->GetPos().y, -pShadowCamera->GetPos().z);
	shadowCameraRotateTrans.InitCameraTransform(pShadowCamera->GetTarget(), pShadowCamera->GetUp());
	shadow_Vtransformation = shadowCameraRotateTrans * shadowCameraTranslationTrans;
	shadow_VP = m_ProjTransformation * shadow_Vtransformation;
	shadow_WVP = shadow_VP * m_Wtransformation;
	glUniformMatrix4fv(gLightWVP, 1, GL_TRUE, (const GLfloat*)shadow_WVP);

	pMesh2->Render();

	//绘制物体2
	Vector3f pmesh2pos(1.0f, 5.5f, 4.0f);
	ScaleTrans.InitScaleTransform(0.1f, 0.1f, 0.1f);
	RotateTrans.InitRotateTransform(30.0f, 50.0f, 25.0f);
	TranslationTrans.InitTranslationTransform(pmesh2pos.x, pmesh2pos.y, pmesh2pos.z);
	m_Wtransformation = TranslationTrans * RotateTrans * ScaleTrans;
	//m_VPtransformation = m_ProjTransformation * m_Vtransformation;//该矩阵未发生改变
	m_WVPtransformation = m_VPtransformation * m_Wtransformation;
	glUniformMatrix4fv(gWorld, 1, GL_TRUE, (const GLfloat*)m_Wtransformation);
	glUniformMatrix4fv(gWVPLocation, 1, GL_TRUE, (const GLfloat*)m_WVPtransformation);

	//为物体2调整灯光矩阵
	pShadowCamera->SetTarget(pmesh2pos);
	//初始化灯光相机相关矩阵	
	shadowCameraTranslationTrans.InitTranslationTransform(-pShadowCamera->GetPos().x, -pShadowCamera->GetPos().y, -pShadowCamera->GetPos().z);
	shadowCameraRotateTrans.InitCameraTransform(pShadowCamera->GetTarget(), pShadowCamera->GetUp());
	shadow_Vtransformation = shadowCameraRotateTrans * shadowCameraTranslationTrans;
	shadow_VP = m_ProjTransformation * shadow_Vtransformation;
	shadow_WVP = shadow_VP * m_Wtransformation;
	glUniformMatrix4fv(gLightWVP, 1, GL_TRUE, (const GLfloat*)shadow_WVP);

	pMesh->Render();

	//绘制物体3
	Vector3f pmesh3pos(-5.0f, -2.5f, -5.0f);
	pTexture->Bind();
	ScaleTrans.InitScaleTransform(20.0f, 0.0f, 20.0f);
	RotateTrans.InitRotateTransform(0.0f, 0.0f, 0.0f);
	TranslationTrans.InitTranslationTransform(pmesh3pos.x, pmesh3pos.y, pmesh3pos.z);
	m_Wtransformation = TranslationTrans * RotateTrans * ScaleTrans;
	//m_VPtransformation = m_ProjTransformation * m_Vtransformation;
	m_WVPtransformation = m_VPtransformation * m_Wtransformation;
	glUniformMatrix4fv(gWorld, 1, GL_TRUE, (const GLfloat*)m_Wtransformation);
	glUniformMatrix4fv(gWVPLocation, 1, GL_TRUE, (const GLfloat*)m_WVPtransformation);

	//为物体3调整灯光矩阵
	pShadowCamera->SetTarget(pmesh3pos);
	//初始化灯光相机相关矩阵	
	shadowCameraTranslationTrans.InitTranslationTransform(-pShadowCamera->GetPos().x, -pShadowCamera->GetPos().y, -pShadowCamera->GetPos().z);
	shadowCameraRotateTrans.InitCameraTransform(pShadowCamera->GetTarget(), pShadowCamera->GetUp());
	shadow_Vtransformation = shadowCameraRotateTrans * shadowCameraTranslationTrans;
	shadow_VP = m_ProjTransformation * shadow_Vtransformation;
	shadow_WVP = shadow_VP * m_Wtransformation;
	glUniformMatrix4fv(gLightWVP, 1, GL_TRUE, (const GLfloat*)shadow_WVP);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)20);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

	glDrawElements(GL_TRIANGLES, length2, GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

static void RenderSceneCB()
{
	ShadowMapPass();
	RenderPass();
	glutSwapBuffers();
}

static void PassiveMouseCB(int x, int y)
{
	pCamera->OnMouse(x, y);
}

static void KeyboardCB(unsigned char Key,int x,int y)
{
	std::cout << "Key:" << Key << std::endl;
}

static void InitializeGlutCallbacks()
{
	glutDisplayFunc(RenderSceneCB);
	glutIdleFunc(RenderSceneCB);
	glutPassiveMotionFunc(PassiveMouseCB);
	glutKeyboardFunc(KeyboardCB);
}

static void CompileShaders()
{
	ShaderProgram = glCreateProgram();
	if (ShaderProgram == 0)
	{
		std::cout << "Error creating shader program\n" << std::endl;
		exit(1);
	}
	std::string vs, fs;
	if (!ReadFile(pVSFileName,vs))
	{
		exit(1);
	}
	if (!ReadFile(pFSFileName, fs))
	{
		exit(1);
	}
	Shader vsShader(vs, GL_VERTEX_SHADER);
	Shader fsShader(fs, GL_FRAGMENT_SHADER);
	AddShader(ShaderProgram, vsShader);
	AddShader(ShaderProgram, fsShader);
	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };
	glLinkProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);
	if (Success == 0) {
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
		exit(1);
	}
	glValidateProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
		exit(1);
	}
	glUseProgram(ShaderProgram);
	GetParamsInShader(ShaderProgram);
}

static void CompileShadowShader()
{
	ShadowShaderProgram = glCreateProgram();
	if (ShadowShaderProgram == 0)
	{
		std::cout << "Error creating shader program\n" << std::endl;
		exit(1);
	}
	std::string vs, fs;
	if (!ReadFile(pShadowVSFileName, vs))
	{
		exit(1);
	}
	if (!ReadFile(pShadowFSFileName, fs))
	{
		exit(1);
	}
	Shader vsShader(vs, GL_VERTEX_SHADER);
	Shader fsShader(fs, GL_FRAGMENT_SHADER);
	AddShader(ShadowShaderProgram, vsShader);
	AddShader(ShadowShaderProgram, fsShader);
	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };
	glLinkProgram(ShadowShaderProgram);
	glGetProgramiv(ShadowShaderProgram, GL_LINK_STATUS, &Success);
	if (Success == 0) {
		glGetProgramInfoLog(ShadowShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
		exit(1);
	}
	glValidateProgram(ShadowShaderProgram);
	glGetProgramiv(ShadowShaderProgram, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(ShadowShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
		exit(1);
	}
	gShadowWVP = glGetUniformLocation(ShadowShaderProgram, "");
	gShadowTex = glGetUniformLocation(ShadowShaderProgram, "");
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
		//fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return 1;
	}

	LightInit();
	pCamera = new Camera(WINDOW_WIDTH,WINDOW_HEIGHT);
	pTexture = new Texture(0,"test.png");

	shadowMapFBO.Init(WINDOW_WIDTH, WINDOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);//!不bind不行！

	unsigned int Indices[] = { 0, 3, 1,
							1, 3, 2,
							2, 3, 0,
							1, 2, 0 };

	length2 = CreateIndexBuffer(&IBO, Indices, sizeof(Indices));
	length = CreateVertexBuffer(&VBO, Indices, 12);

	pMesh = new Mesh();
	pMesh->LoadMesh("Content/phoenix_ugv.md2");

	pMesh2 = new Mesh();
	pMesh2->LoadMesh("Content/phoenix_ugv.md2");

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	//length=CreateVertexBuffer(&VBO);
	//length2 = CreateIndexBuffer(&IBO);
	
	//std::cout << "length2" << length2 << std::endl;
	CompileShaders();
	CompileShadowShader();

	gPersProjInfo.FOV = 60.0f;
	gPersProjInfo.Height = WINDOW_HEIGHT;
	gPersProjInfo.Width = WINDOW_WIDTH;
	gPersProjInfo.zNear = 1.0f;
	gPersProjInfo.zFar = 100.0f;

	//初始化透视投影矩阵
	m_ProjTransformation.InitPersProjTransform(gPersProjInfo);

	//初始化相机相关矩阵
	Matrix4f CameraTranslationTrans, CameraRotateTrans;
	CameraTranslationTrans.InitTranslationTransform(-pCamera->GetPos().x, -pCamera->GetPos().y, -pCamera->GetPos().z);
	CameraRotateTrans.InitCameraTransform(pCamera->GetTarget(), pCamera->GetUp());
	m_Vtransformation = CameraRotateTrans * CameraTranslationTrans;

	//初始化物体相关矩阵
	Matrix4f ScaleTrans, RotateTrans, TranslationTrans;
	ScaleTrans.InitScaleTransform(1.0f, 1.0f, 1.0f);
	RotateTrans.InitRotateTransform(0.0f, 0.0f, 0.0f);
	TranslationTrans.InitTranslationTransform(0.5f, 0.4f, 3.7f);
	m_Wtransformation = TranslationTrans * RotateTrans * ScaleTrans;	
	m_VPtransformation = m_ProjTransformation * m_Vtransformation;
	m_WVPtransformation = m_VPtransformation * m_Wtransformation;

	std::cout << m_ProjTransformation.m[0][0] << " " << m_ProjTransformation.m[0][1] << " " << m_ProjTransformation.m[0][2] << " " << m_ProjTransformation.m[0][3] << std::endl;
	std::cout << m_ProjTransformation.m[1][0] << " " << m_ProjTransformation.m[1][1] << " " << m_ProjTransformation.m[1][2] << " " << m_ProjTransformation.m[1][3] << std::endl;
	std::cout << m_ProjTransformation.m[2][0] << " " << m_ProjTransformation.m[2][1] << " " << m_ProjTransformation.m[2][2] << " " << m_ProjTransformation.m[2][3] << std::endl;
	std::cout << m_ProjTransformation.m[3][0] << " " << m_ProjTransformation.m[3][1] << " " << m_ProjTransformation.m[3][2] << " " << m_ProjTransformation.m[3][3] << std::endl;
	std::cout << std::endl;

	std::cout << m_Vtransformation.m[0][0] << " " << m_Vtransformation.m[0][1] << " " << m_Vtransformation.m[0][2] << " " << m_Vtransformation.m[0][3] << std::endl;
	std::cout << m_Vtransformation.m[1][0] << " " << m_Vtransformation.m[1][1] << " " << m_Vtransformation.m[1][2] << " " << m_Vtransformation.m[1][3] << std::endl;
	std::cout << m_Vtransformation.m[2][0] << " " << m_Vtransformation.m[2][1] << " " << m_Vtransformation.m[2][2] << " " << m_Vtransformation.m[2][3] << std::endl;
	std::cout << m_Vtransformation.m[3][0] << " " << m_Vtransformation.m[3][1] << " " << m_Vtransformation.m[3][2] << " " << m_Vtransformation.m[3][3] << std::endl;
	std::cout << std::endl;

	std::cout << m_Wtransformation.m[0][0] << " " << m_Wtransformation.m[0][1] << " " << m_Wtransformation.m[0][2] << " " << m_Wtransformation.m[0][3] << std::endl;
	std::cout << m_Wtransformation.m[1][0] << " " << m_Wtransformation.m[1][1] << " " << m_Wtransformation.m[1][2] << " " << m_Wtransformation.m[1][3] << std::endl;
	std::cout << m_Wtransformation.m[2][0] << " " << m_Wtransformation.m[2][1] << " " << m_Wtransformation.m[2][2] << " " << m_Wtransformation.m[2][3] << std::endl;
	std::cout << m_Wtransformation.m[3][0] << " " << m_Wtransformation.m[3][1] << " " << m_Wtransformation.m[3][2] << " " << m_Wtransformation.m[3][3] << std::endl;
	std::cout << std::endl;

	std::cout << m_WVPtransformation.m[0][0] << " " << m_WVPtransformation.m[0][1] << " " << m_WVPtransformation.m[0][2] << " " << m_WVPtransformation.m[0][3] << std::endl;
	std::cout << m_WVPtransformation.m[1][0] << " " << m_WVPtransformation.m[1][1] << " " << m_WVPtransformation.m[1][2] << " " << m_WVPtransformation.m[1][3] << std::endl;
	std::cout << m_WVPtransformation.m[2][0] << " " << m_WVPtransformation.m[2][1] << " " << m_WVPtransformation.m[2][2] << " " << m_WVPtransformation.m[2][3] << std::endl;
	std::cout << m_WVPtransformation.m[3][0] << " " << m_WVPtransformation.m[3][1] << " " << m_WVPtransformation.m[3][2] << " " << m_WVPtransformation.m[3][3] << std::endl;	

	glutMainLoop();

	return 0;
}