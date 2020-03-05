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