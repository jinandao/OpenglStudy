#include <glew.h>
#include <freeglut.h>
#include <iostream>
#include "OpenglTools.h"
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
#include "mesh.h"

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

GLuint VBO;
GLuint IBO;

int length;
int length2;

const char* pVSFileName = "shader.vs";
const char* pFSFileName = "shader.fs";
//GLuint gScaleLocation;//对shader参数的调整需要怎么封装？
GLuint gSampler;
GLuint gWVPLocation;
PersProjInfo gPersProjInfo;

Texture* pTexture=nullptr;
Camera* pCamera = nullptr;
Mesh* pMesh = nullptr;

Matrix4f m_WVPtransformation;
Matrix4f m_VPtransformation;
Matrix4f m_WPtransformation;
Matrix4f m_WVtransformation;
Matrix4f m_Wtransformation;
Matrix4f m_Vtransformation;
Matrix4f m_ProjTransformation;

struct RenderItem
{
	Vector3f scale;
	Vector3f worldPos;
	Vector3f rotateInfo;
};

static void RenderSceneCB()
{
	glClear(GL_COLOR_BUFFER_BIT);

	static float Scale = 0.0f;
	Scale += 0.001f;
	//glUniform1f(gScaleLocation, sinf(Scale));

	pCamera->OnRender();

	//初始化相机相关矩阵
	Matrix4f CameraTranslationTrans, CameraRotateTrans;
	CameraTranslationTrans.InitTranslationTransform(-pCamera->GetPos().x, -pCamera->GetPos().y, -pCamera->GetPos().z);
	CameraRotateTrans.InitCameraTransform(pCamera->GetTarget(), pCamera->GetUp());
	m_Vtransformation = CameraRotateTrans * CameraTranslationTrans;

	//初始化物体相关矩阵
	Matrix4f ScaleTrans, RotateTrans, TranslationTrans;
	ScaleTrans.InitScaleTransform(1.0f, 1.0f, 1.0f);
	RotateTrans.InitRotateTransform(0.0f, 0.0f, 0.0f);
	TranslationTrans.InitTranslationTransform(1.0f, 0.5f, 15.0f);
	m_Wtransformation = TranslationTrans * RotateTrans * ScaleTrans;

	m_VPtransformation = m_ProjTransformation * m_Vtransformation;

	m_WVPtransformation = m_VPtransformation * m_Wtransformation;

	glUniformMatrix4fv(gWVPLocation, 1, GL_TRUE, (const GLfloat*)m_WVPtransformation);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//std::cout << "size:" << sizeof(Vertex) << std::endl;
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 20, (const GLvoid*)12);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	pTexture->Bind(0);

	//std::cout << length2 << std::endl;
	//glDrawElements(GL_TRIANGLES, length2,GL_UNSIGNED_INT,0);

	//Matrix4f ScaleTrans, RotateTrans, TranslationTrans;
	ScaleTrans.InitScaleTransform(0.2f, 0.2f, 0.2f);
	RotateTrans.InitRotateTransform(30.0f, 30.0f, 30.0f);
	TranslationTrans.InitTranslationTransform(0.0f, 0.0f, 30.0f);
	m_Wtransformation = TranslationTrans * RotateTrans * ScaleTrans;
	m_WVPtransformation = m_VPtransformation * m_Wtransformation;

	glUniformMatrix4fv(gWVPLocation, 1, GL_TRUE, (const GLfloat*)m_WVPtransformation);

	pMesh->Render();

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

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
	GLuint ShaderProgram = glCreateProgram();
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

	//gScaleLocation = glGetUniformLocation(ShaderProgram, "gScale");
	gSampler = glGetUniformLocation(ShaderProgram, "gSampler");
	//assert(gScaleLocation != 0xFFFFFFFF);
	gWVPLocation = glGetUniformLocation(ShaderProgram, "gWVP");
	if (gSampler == 0xFFFFFFFF)
	{
		std::cout << "gSampler not found" << std::endl;
	}
	if (gWVPLocation == 0xFFFFFFFF)
	{
		std::cout << "gWVPLocation not found" << std::endl;
	}
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

	pCamera = new Camera(WINDOW_WIDTH,WINDOW_HEIGHT);
	pTexture = new Texture(0,"test.png");
	pMesh = new Mesh();
	pMesh->LoadMesh("Content/phoenix_ugv.md2");

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	length=CreateVertexBuffer(&VBO);
	length2 = CreateIndexBuffer(&IBO);
	std::cout << "length2" << length2 << std::endl;
	CompileShaders();

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

	glUniform1i(gSampler, 0);

	glutMainLoop();

	return 0;
}