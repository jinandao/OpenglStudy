#include "BarrageShoot.h"
#include "../GameLayer/Avatar.h"
#include "../EngineLayer/engine_common.h"
#include "../GameLayer/Pipeline.h"
#include "Enemy.h"
#include "EnemyAI.h"

int BarriageBullet::BulletSpeed = 2;

//extern std::vector<std::pair<Enemy*, EnemyAI*>> enemies;
//extern Enemy* enemy3;

bool BarriageBullet::CheckIsIn(Vector3f selfPos, std::vector<std::pair<Enemy*, EnemyAI*>>& enemies, Enemy*& boss)
{
	for (int i = 0; i < enemies.size(); i++)
	{
		if (Distance(selfPos, enemies[i].first->GetPos()) < 2)
		{
			if (enemies[i].first->isDead == false)
			{
				enemies[i].first->TakeHurt();
				return true;
			}			
		}			
	}
	if (boss->isDead==false)
	{
		if (Distance(selfPos, boss->GetPos()) < 2)
		{
			boss->TakeHurt();
			return true;
		}
	}
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

Barriage::Barriage():updateTimes(100), intervalTimes(200)
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

void Barriage::Update(Pipeline p,std::vector<std::pair<Enemy*, EnemyAI*>> &enemies,Enemy* &boss)
{
	if (isFired == true)
	{
		if (intervalTimes > 0)
		{
			intervalTimes -= 1;
		}
		else
		{
			isFired = false;
		}
		if (updateTimes > 0)
		{
			updateTimes -= 1;
			for (unsigned int i = 0; i < NUM_INSTANCES; i++)
			{
				BarriageBullet::BulletMove(bulletsPos[i], bulletRotations[i]);
				if (BarriageBullet::CheckIsIn(bulletsPos[i], enemies,boss))
				{
					shouldRenders[i] = 1;
				}
			}
			Render(p);
		}
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
		//shouldRenders[i] = 0;
	}

	mesh->Render(NUM_INSTANCES, WVPMatrics, WorldMatrics,shouldRenders);
}

void Barriage::Shoot(Vector3f pos, Vector3f rotate)
{
	if (isFired == false)
	{
		isFired = true;
		updateTimes = 100;
		intervalTimes = 200;
		for (int i = 0; i < NUM_INSTANCES; i++)
		{
			shouldRenders[i] = 0;
			bulletsPos[i] = pos;
			bulletRotations[i].y = rotate.y - NUM_INSTANCES / 2 * 10 + i * 10;
		}
	}	
}

