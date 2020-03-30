#include "Bullet.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemyAI.h"

extern Player* player;
extern std::vector<std::pair<Enemy*, EnemyAI*>> enemies;

void Bullet::Init()
{
	geo = std::shared_ptr<Geometry>(new Geometry(1, 1));
	Vertex Vertices[4] = { Vertex(Vector3f(0.0f, 0.0f, 0.0f), Vector2f(0.0f, 0.0f),Vector3f(0.0f,1.0f,0.0f)),
						   Vertex(Vector3f(0.0f, 0.0f, 1.0f), Vector2f(0.0f, 1.0f),Vector3f(0.0f,1.0f,0.0f)),
						   Vertex(Vector3f(1.0f, 0.0f, 0.0f), Vector2f(1.0f, 0.0f),Vector3f(0.0f,1.0f,0.0f)),
						   Vertex(Vector3f(1.0f, 0.0f, 1.0f), Vector2f(1.0f, 1.0f),Vector3f(0.0f,1.0f,0.0f)) };
	unsigned int Indices[] = { 0, 3, 1,
							1, 3, 2,
							2, 3, 0,
							1, 2, 0 };
	std::vector<Vertex> verts(4);
	for (int i = 0; i < 4; i++)
	{
		verts[i] = Vertices[i];
	}
	std::vector<unsigned int> indices(12);
	for (int i = 0; i < 12; i++)
	{
		indices[i] = Indices[i];
	}
	geo->AddMeshEntry(verts, indices, 0);
	std::string testpic2("Content/test3.png");
	geo->AddTexure(3, testpic2);
}

void Bullet::Move()
{
	if (timer < 3)
	{
		timer += 0.01;
		float angle = rotation.y;
		PositionChange(speed * cosf(ToRadian(angle + 90)), 0, speed * sinf(ToRadian(angle + 90)));
	}
	else
	{
		isEnd = true;
	}
}

void Bullet::CheckHurt()
{
	if (isPlayer)
	{
		for (auto ite = enemies.begin(); ite != enemies.end();)
		{
			if (Distance(ite->first->GetPos(), this->GetPos()) < 2)
			{
				isEnd = true;
				if (ite->first->isDead == false)
				{
					ite->first->TakeHurt();
				}
				ite++;
			}
			else
				ite++;
		}
	}
	else
	{
		if (player != nullptr)
		{
			if (Distance(player->GetPos(), this->GetPos()) < 2)
			{
				isEnd = true;
				player->TakeHurt();
			}
		}
	}
}
