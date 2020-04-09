#pragma once

#include "../EngineLayer/InstancedRenderTechnique.h"
#include "../EngineLayer/InstancedMesh.h"
#include "../EngineLayer/MathTools.h"

#include <vector>

#define NUM_INSTANCES 11

class Pipeline;
class Avatar;

class BarriageBullet
{
public:
	static int BulletSpeed;
	static bool CheckIsIn(Vector3f selfPos,Vector3f targetPos);
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
	static int updateTimes;//可以在多少次循环中存在
	static int intervalTimes;//需要间隔多少次循环才可以释放一次
	
	int shouldRenders[NUM_INSTANCES];//哪些子弹应该被绘制，被碰撞的子弹，将该位置1，则不再绘制，否则置0，继续绘制
	Matrix4f WVPMatrics[NUM_INSTANCES];
	Matrix4f WorldMatrics[NUM_INSTANCES];

	void Render(Pipeline p);

public:
	Barriage();
	~Barriage();
	bool LoadBarriage(const std::string fileName);
	InstancedMesh* GetMesh();
	void Update(Pipeline p);
	void BulletInitial(Vector3f pos, Vector3f rotate);
	static void Shoot();
	static bool isFired;
};