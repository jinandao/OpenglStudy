#include "BarrageShoot.h"
#include "../GameLayer/Avatar.h"
#include "../EngineLayer/engine_common.h"
#include "../GameLayer/Pipeline.h"

int BarriageBullet::BulletSpeed = 2;
int Barriage::updateTimes = 200;
int Barriage::intervalTimes = 300;
bool Barriage::isFired = true;

bool BarriageBullet::CheckIsIn(Vector3f selfPos, Vector3f targetPos)
{
	return false;
}

void BarriageBullet::TakeHurt(Avatar* avatar)
{
}

void BarriageBullet::BulletMove(Vector3f& pos, const Vector3f rotate)
{
	float angle = rotate.y;

	pos.x += BulletSpeed * cosf(ToRadian(angle + 90));
	pos.z += BulletSpeed * sinf(ToRadian(angle + 90));
}

Barriage::Barriage()
{
	mesh = new InstancedMesh();
	instanceTechnique = new InstancedLightingTechnique();
	//mesh->LoadMesh()
	bulletsPos.resize(NUM_INSTANCES);
	bulletRotations.resize(NUM_INSTANCES);
	//shouldRenders.resize(NUM_INSTANCES);
}

Barriage::~Barriage()
{
	if (instanceTechnique != nullptr)
	{
		delete instanceTechnique;
		instanceTechnique = nullptr;
	}
	if (mesh != nullptr)
	{
		delete mesh;
		mesh = nullptr;
	}
}

bool Barriage::LoadBarriage(const std::string fileName)
{
	return mesh->LoadMesh(fileName);
}

InstancedMesh* Barriage::GetMesh()
{
	return nullptr;
}

void Barriage::Update(Pipeline p)
{
	if (isFired == true)
	{
		if (updateTimes > 0)
		{
			updateTimes -= 1;
		}
		else
		{
			isFired = false;
		}
		for (unsigned int i = 0; i < NUM_INSTANCES; i++)
		{
			BarriageBullet::BulletMove(bulletsPos[i], bulletRotations[i]);
		}		
		Render(p);
	}
}

void Barriage::Render(Pipeline p)
{
	instanceTechnique->Enable();

	for (unsigned int i = 0; i < NUM_INSTANCES; i++)
	{
		p.WorldPos(bulletsPos[i]);
		p.Rotate(bulletRotations[i]);
		p.Scale(0.1, 0.1, 0.1);

		WVPMatrics[i] = p.GetWVPTrans().Transpose();
		WorldMatrics[i] = p.GetWorldTrans().Transpose();
		shouldRenders[i] = 0;
	}

	shouldRenders[2] = 1;
	shouldRenders[6] = 1;
	shouldRenders[9] = 1;

	mesh->Render(NUM_INSTANCES, WVPMatrics, WorldMatrics,shouldRenders);
}

void Barriage::BulletInitial(Vector3f pos, Vector3f rotate)
{
	for (int i = 0; i < NUM_INSTANCES; i++)
	{
		bulletsPos[i] = pos;
		bulletRotations[i].y = rotate.y-NUM_INSTANCES/2*10+i * 10;
	}
}

void Barriage::Shoot()
{
	isFired = true;
	updateTimes = 200;
}

