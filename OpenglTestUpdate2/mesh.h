/*
����������صĴ����ļ�����������ģ�͡��Զ������ģ��
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


class Geometry:public DrawObject//�����Զ���ģ�ͣ���ƽ���
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

class Mesh:public DrawObject//��Ҫ���ļ����صĸ���ģ��
{
private:	

	bool InitFromScene(const aiScene* pScene, const std::string& Filename);
	void InitMesh(unsigned int Index, const aiMesh* paiMesh);
	bool InitMaterials(const aiScene* pScene, const std::string& Filename);
	void Clear();

	static int meshNums;//��¼���ж��ٸ�ģ�ͱ�����

public:
	Mesh();
	~Mesh();

	bool LoadMesh(const std::string& Filename);

};

