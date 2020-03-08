#include "OpenglTools.h"
#include <Windows.h>
//#include <string>
#include <vector>
#include <iostream>
#include <fstream>

int CreateVertexBuffer(GLuint* VBO)
{
	/*Vector3f Vertices[9];
	Vertices[0] = Vector3f(-1.0f, -1.0f, 0.0f);
	Vertices[1] = Vector3f(0.5f, -0.5f, 0.0f);
	Vertices[2] = Vector3f(0.0f,0.5f, 0.0f);
	Vertices[3] = Vector3f(0.5f, 0.5f, 0.0f);
	Vertices[4] = Vector3f(0.0f, -1.0f, 0.0f);
	Vertices[5] = Vector3f(0.5f, 0.0f, 0.0f);

	Vertices[6] = Vector3f(0.5f, 0.8f, 0.0f);
	Vertices[7] = Vector3f(0.6f, -1.0f, 0.0f);
	Vertices[8] = Vector3f(0.5f, 0.3f, 0.0f);*/

	Vertex Vertices[4] = { Vertex(Vector3f(-1.0f, -1.0f, 0.5773f), Vector2f(0.0f, 0.0f)),
						   Vertex(Vector3f(0.0f, -1.0f, -1.15475f), Vector2f(0.0f, 1.0f)),
						   Vertex(Vector3f(1.0f, -1.0f, 0.5773f),  Vector2f(1.0f, 0.0f)),
						Vertex(Vector3f(0.0f, 1.0f, 0.0f),Vector2f(1.0f,1.0f))};

	/*float Vertices[20] = { -1.0f, -1.0f, 0.5773f,0.0f, 0.0f,
		0.0f, -1.0f, -1.15475f,0.0f, 1.0f,
		1.0f, -1.0f, 0.5773f,1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,1.0f,1.0f };*/
	glGenBuffers(1, VBO);
	glBindBuffer(GL_ARRAY_BUFFER, *VBO);
	//std::cout << "size0:" << sizeof(Vertices) << std::endl;
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

	//return sizeof(Vertices) / sizeof(Vector3f);
	return sizeof(Vertices)/sizeof(Vertex);
}

int CreateIndexBuffer(GLuint* IBO)
{
	unsigned int Indices[] = { 0, 2, 1,
								0,1,3};

	glGenBuffers(1, IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);

	return sizeof(Indices) / sizeof(unsigned int);
}

int CreateIndexBuffer(GLuint* IBO, const unsigned int* pIndices, unsigned int SizeInBytes)
{
	glGenBuffers(1, IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, SizeInBytes, pIndices, GL_STATIC_DRAW);
	return SizeInBytes/sizeof(unsigned int);
}

int CreateVertexBuffer(GLuint* VBO,const unsigned int* pIndices, unsigned int IndexCount)
{
	Vertex Vertices[4] = { Vertex(Vector3f(0.0f, 0.0f, 0.0f), Vector2f(0.0f, 0.0f),Vector3f(0.0f,1.0f,0.0f)),
							   Vertex(Vector3f(0.0f, 0.0f, 1.0f), Vector2f(0.0f, 1.0f),Vector3f(0.0f,1.0f,0.0f)),
							   Vertex(Vector3f(1.0f, 0.0f, 0.0f), Vector2f(1.0f, 0.0f),Vector3f(0.0f,1.0f,0.0f)),
							   Vertex(Vector3f(1.0f, 0.0f, 1.0f), Vector2f(1.0f, 1.0f),Vector3f(0.0f,1.0f,0.0f)) };


	//unsigned int VertexCount = ARRAY_SIZE_IN_ELEMENTS(Vertices);
	//CalcNormals(pIndices, IndexCount, Vertices, VertexCount);

	std::cout <<"normal:"<< Vertices[0].normal.x << " " << Vertices[0].normal.y << " " << Vertices[0].normal.z << std::endl;
	std::cout <<"normal:"<< Vertices[1].normal.x << " " << Vertices[1].normal.y << " " << Vertices[1].normal.z << std::endl;
	std::cout <<"normal:"<< Vertices[2].normal.x << " " << Vertices[2].normal.y << " " << Vertices[2].normal.z << std::endl;
	std::cout <<"normal:"<< Vertices[3].normal.x << " " << Vertices[3].normal.y << " " << Vertices[3].normal.z << std::endl;

	glGenBuffers(1, VBO);
	glBindBuffer(GL_ARRAY_BUFFER, *VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
	return sizeof(Vertices) / sizeof(Vertex);
}

bool ReadFile(const char* pFileName, std::string& outFile)
{
	std::ifstream f(pFileName);
	bool ret = false;
	if (f.is_open()) {
		std::string line;
		while (getline(f, line))
		{
			outFile.append(line);
			outFile.append("\n");
		}
		f.close();
		ret = true;
	}
	else {
		std::cout << "Error read file: " << pFileName << std::endl;		
	}
	return ret;
}

void CalcNormals(const unsigned int* pIndices, unsigned int IndexCount, Vertex* pVertices, unsigned int VertexCount)
{
	for (unsigned int i = 0; i < IndexCount; i += 3) {
		unsigned int Index0 = pIndices[i];
		unsigned int Index1 = pIndices[i + 1];
		unsigned int Index2 = pIndices[i + 2];
		Vector3f v1 = pVertices[Index1].pos - pVertices[Index0].pos;
		Vector3f v2 = pVertices[Index2].pos - pVertices[Index0].pos;
		Vector3f Normal = v1.Cross(v2);
		Normal.Normalize();
		pVertices[Index0].normal += Normal;
		pVertices[Index1].normal += Normal;
		pVertices[Index2].normal += Normal;
	}
	// Normalize all the vertex normals
	for (unsigned int i = 0; i < VertexCount; i++) {
		pVertices[i].normal.Normalize();
	}
}
