/*
opengl函数封装代码文件
*/

#pragma once
#include <glew.h>
#include <freeglut.h>
#include <string>
#include "MathTools.h"

#define INVALID_UNIFORM_LOCATION 0xffffffff
#define INVALID_OGL_VALUE 0xffffffff
#define INVALID_MATERIAL 0xFFFFFFFF
#define ASSIMP_LOAD_FLAGS (aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices)

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

struct Vertex
{
	Vector3f pos;
	Vector2f tex;
	
	Vector3f normal;
	
	Vertex(){}
	Vertex(Vector3f _pos,Vector2f _tex):pos(_pos),tex(_tex)
	{}
	Vertex(Vector3f _pos, Vector2f _tex,Vector3f _normal) :pos(_pos), tex(_tex),normal(_normal)
	{}
};

void CreateQuad();

void CreateBox();

int CreateVertexBuffer(GLuint* VBO);

int CreateIndexBuffer(GLuint* IBO);

int CreateIndexBuffer(GLuint* IBO,const unsigned int* pIndices, unsigned int SizeInBytes);

int CreateVertexBuffer(GLuint* VBO,const unsigned int* pIndices, unsigned int IndexCount);

void GetParamsInShader(GLuint shader, GLuint& shaderParam, const char* paramName);

bool ReadFile(const char* fileName, std::string& outFile);

void CalcNormals(const unsigned int* pIndices, unsigned int IndexCount,
	Vertex* pVertices, unsigned int VertexCount);

Matrix4f GetWMatrixForObject(Vector3f Scale, Vector3f Rotate, Vector3f Translation);

Matrix4f GetWMatrixForCamera(Vector3f Target, Vector3f Up, Vector3f Transition);

void DrawCall(GLuint& VBO, GLuint& IBO, int length);