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

//struct Vertex
//{
//	Vector3f pos;
//	Vector2f tex;
//	Vertex() {}
//	Vertex(Vector3f _pos, Vector2f _tex) :pos(_pos), tex(_tex)
//	{}
//};

class Mesh
{
private:
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
	std::vector<MeshEntry> m_Entries;
	std::vector<Texture*> m_Textures;

	bool InitFromScene(const aiScene* pScene, const std::string& Filename);
	void InitMesh(unsigned int Index, const aiMesh* paiMesh);
	bool InitMaterials(const aiScene* pScene, const std::string& Filename);
	void Clear();

public:
	Mesh();
	~Mesh();

	bool LoadMesh(const std::string& Filename);

	void Render();
};

