#pragma once

#include "../EngineLayer/InstancedRenderTechnique.h"
#include "../EngineLayer/InstancedMesh.h"
#include "../EngineLayer/MathTools.h"

#include <vector>

#define NUM_INSTANCES 11

class Pipeline;
class Avatar;
class Enemy;
class EnemyAI;

class BarriageBullet
{
public:
	static int BulletSpeed;
	static bool CheckIsIn(Vector3f selfPos, std::vector<std::pair<Enemy*, EnemyAI*>>& enemies, Enemy*& boss);
	static void TakeHurt(Avatar* avatar);
	static void BulletMove(Vector3f& pos, const Vector3f rotate);
};

class Barriage
{
private:
	InstancedMesh* mesh;
	InstancedLightingTechnique* instanceTechnique;
	std::vector<Vector3f> bulletsPos;
	std::vector<Vector3f> bulletRotations;
	//std::vector<int> shouldRenders;
	int updateTimes;//�����ڶ��ٴ�ѭ���д���
	int intervalTimes;//��Ҫ������ٴ�ѭ���ſ����ͷ�һ��
	
	int shouldRenders[NUM_INSTANCES];//��Щ�ӵ�Ӧ�ñ����ƣ�����ײ���ӵ�������λ��1�����ٻ��ƣ�������0����������
	Matrix4f WVPMatrics[NUM_INSTANCES];
	Matrix4f WorldMatrics[NUM_INSTANCES];

	void Render(Pipeline p);

public:
	Barriage();
	~Barriage();
	bool LoadBarriage(const std::string fileName);
	InstancedMesh* GetMesh();
	void Update(Pipeline p, std::vector<std::pair<Enemy*, EnemyAI*>>& enemies, Enemy*& boss);
	void Shoot(Vector3f pos, Vector3f rotate);
	bool isFired;
};