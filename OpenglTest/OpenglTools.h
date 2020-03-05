#pragma once
#include <glew.h>
#include <freeglut.h>
#include <string>
#include "MathTools.h"

#define INVALID_UNIFORM_LOCATION 0xffffffff
#define INVALID_OGL_VALUE 0xffffffff
#define INVALID_MATERIAL 0xFFFFFFFF
#define ASSIMP_LOAD_FLAGS (aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices)


struct Vertex
{
	Vector3f pos;
	Vector2f tex;
	Vertex(){}
	Vertex(Vector3f _pos,Vector2f _tex):pos(_pos),tex(_tex)
	{}
};

int CreateVertexBuffer(GLuint* VBO);

int CreateIndexBuffer(GLuint* IBO);

bool ReadFile(const char* fileName, std::string& outFile);
