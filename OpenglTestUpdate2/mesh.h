/*
绘制物体相关的代码文件，包括加载模型、自定义简易模型
*/

#pragma once

#include <map>
#include <vector>
#include <glew.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "MathTools.h"
#include "Texture.h"
#include "OpenglTools.h"

struct MeshEntry
{
	GLuint VB;
	GLuint IB;
	unsigned int NumIndices;
	unsigned int MaterialIndex;
	MeshEntry();
	~MeshEntry();
	void Init(const std::vector<Vertex>& Vertices,
		const std::vector<unsigned int>& Indices);
};

class DrawObject
{
protected:
	std::vector<MeshEntry> m_Entries;
	std::vector<Texture*> m_Textures;

public:	
	void Render() const;
};


class Geometry:public DrawObject//简易自定义模型，如平面等
{
private:
	
	int entryNum;
	int texNum;
public:
	Geometry(int _entryNum,int _texNum):entryNum(0),texNum(0)
	{
		m_Entries.resize(_entryNum);
		m_Textures.resize(_texNum);
	}
	~Geometry();
	void AddMeshEntry(const std::vector<Vertex>& verts,const std::vector<unsigned int>& indices,int matIndex);
	void AddTexure(int id, std::string& path);
};

class Mesh:public DrawObject//需要从文件加载的复杂模型
{
private:	

	bool InitFromScene(const aiScene* pScene, const std::string& Filename);
	void InitMesh(unsigned int Index, const aiMesh* paiMesh);
	bool InitMaterials(const aiScene* pScene, const std::string& Filename);
	void Clear();

	static int meshNums;//记录了有多少个模型被加载

public:
	Mesh();
	~Mesh();

	bool LoadMesh(const std::string& Filename);

};

