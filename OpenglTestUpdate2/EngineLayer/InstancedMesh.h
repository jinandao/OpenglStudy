#pragma once

#include "mesh.h"

class InstancedMesh
{
private:
#define INVALID_MATERIAL 0xFFFFFFFF

#define INDEX_BUFFER 0    
#define POS_VB       1
#define NORMAL_VB    2
#define TEXCOORD_VB  3    
#define WVP_MAT_VB   4
#define WORLD_MAT_VB 5
#define SHOULD_RENDER_VB 6

	GLuint m_VAO;
	GLuint m_Buffers[7];
	struct MeshEntry {
		unsigned int NumIndices;
		unsigned int BaseVertex;
		unsigned int BaseIndex;
		unsigned int MaterialIndex;
		MeshEntry()
		{
			NumIndices = 0;
			BaseVertex = 0;
			BaseIndex = 0;
			MaterialIndex = INVALID_MATERIAL;
		}
	};
	std::vector<MeshEntry> m_Entries;
	std::vector<Texture*> m_Textures;

public:
	InstancedMesh();
	~InstancedMesh();
	bool LoadMesh(const std::string& Filename);
	void Render(unsigned int NumInstances, const Matrix4f* WVPMats, const Matrix4f* WorldMats, const int* ShouldRenders);

private:
	bool InitFromScene(const aiScene* pScene, const std::string& Filename);
	void InitMesh(const aiMesh* paiMesh,
		std::vector<Vector3f>& Positions,
		std::vector<Vector3f>& Normals,
		std::vector<Vector2f>& TexCoords,
		std::vector<unsigned int>& Indices);

	bool InitMaterials(const aiScene* pScene, const std::string& Filename);
	void Clear();
};