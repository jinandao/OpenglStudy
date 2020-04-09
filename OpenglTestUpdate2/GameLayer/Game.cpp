/*
对游戏中物体的封装
*/

#include "Game.h"
//#include "../GameLogic/GameLogic.h"
#include <iostream>
//#include "../EngineLayer/OpenglTools.h"

//extern Player* player;
//extern std::vector<std::pair<Enemy*, EnemyAI*>> enemies;

//void Avatar::TakeHurt()
//{
//	hp -= 10;
//}
//
//void Avatar::MoveForward()
//{
//	float angle = rotation.y;
//	PositionChange(speed * cosf(ToRadian(angle + 90)), 0, speed * sinf(ToRadian(angle + 90)));
//}
//
//void Avatar::MoveBack()
//{
//	float angle = rotation.y;
//	PositionChange(-speed * cosf(ToRadian(angle + 90)), 0, -speed * sinf(ToRadian(angle + 90)));
//}
//
//void Avatar::TurnRight()
//{
//	RotationChange(0, -angleSpeed, 0);
//}
//
//void Avatar::TurnLeft()
//{
//	RotationChange(0, angleSpeed, 0);
//}
//
//void Avatar::Die()
//{
//	isDead = true;
//}

//void Player::Update()
//{
//
//}
//
//void Player::Shoot(std::vector<Bullet*>& bullets)
//{
//	Bullet* bullet = new Bullet(Vector3f(2, 2, 2), GetRotation(), GetPos(), 3, true);	
//	if (bullets.size()== 0)
//	{		
//		bullet->Init();
//	}
//	else
//	{
//		if (bullets[0] == nullptr)
//			bullet->Init();
//		else
//			bullet->SetGeo(bullets[0]->GetGeo());
//	}	
//	bullets.push_back(bullet);
//}
//
//void Player::TakeHurt()
//{
//	Avatar::TakeHurt();
//	if (hp <= 0)
//	{
//		Die();
//	}
//}
//
//void Player::TakeAction(char c,std::vector<Bullet*>& bullets)
//{
//	if (c == 'w')
//	{
//		MoveForward();
//	}
//	if (c == 's')
//	{
//		MoveBack();
//	}
//	if (c == 'a')
//	{
//		TurnLeft();
//	}
//	if (c == 'd')
//	{
//		TurnRight();
//	}
//	if (c == 'j')
//	{
//		Shoot(bullets);
//	}
//}

//void Enemy::Update()
//{
//
//}

//void Enemy::Shoot(std::vector<Bullet*>& bullets)
//{
//	Bullet* bullet = new Bullet(Vector3f(2, 2, 2), GetRotation(), GetPos(), 1, false);
//	if (bullets.size() == 0)
//	{
//		bullet->Init();
//	}
//	else
//	{
//		if (bullets[0] == nullptr)
//			bullet->Init();
//		else
//			bullet->SetGeo(bullets[0]->GetGeo());
//	}
//	bullets.push_back(bullet);
//}
//
//void Enemy::TakeHurt()
//{
//	Avatar::TakeHurt();
//	if (hp <= 0)
//	{		
//		Die();
//	}
//}

//void Bullet::Init()
//{
//	geo = std::shared_ptr<Geometry>(new Geometry(1, 1));
//	Vertex Vertices[4] = { Vertex(Vector3f(0.0f, 0.0f, 0.0f), Vector2f(0.0f, 0.0f),Vector3f(0.0f,1.0f,0.0f)),
//						   Vertex(Vector3f(0.0f, 0.0f, 1.0f), Vector2f(0.0f, 1.0f),Vector3f(0.0f,1.0f,0.0f)),
//						   Vertex(Vector3f(1.0f, 0.0f, 0.0f), Vector2f(1.0f, 0.0f),Vector3f(0.0f,1.0f,0.0f)),
//						   Vertex(Vector3f(1.0f, 0.0f, 1.0f), Vector2f(1.0f, 1.0f),Vector3f(0.0f,1.0f,0.0f)) };
//	unsigned int Indices[] = { 0, 3, 1,
//							1, 3, 2,
//							2, 3, 0,
//							1, 2, 0 };
//	std::vector<Vertex> verts(4);
//	for (int i = 0; i < 4; i++)
//	{
//		verts[i] = Vertices[i];
//	}
//	std::vector<unsigned int> indices(12);
//	for (int i = 0; i < 12; i++)
//	{
//		indices[i] = Indices[i];
//	}
//	geo->AddMeshEntry(verts, indices, 0);
//	std::string testpic2("Content/test3.png");
//	geo->AddTexure(3, testpic2);
//}

//void Bullet::Move()
//{
//	if (timer < 3)
//	{
//		timer += 0.01;
//		float angle = rotation.y;
//		PositionChange(speed * cosf(ToRadian(angle + 90)), 0, speed * sinf(ToRadian(angle + 90)));
//	}	
//	else
//	{
//		isEnd = true;
//	}
//}
//
//void Bullet::CheckHurt()
//{
//	if(isPlayer)
//	{
//		for (auto ite = enemies.begin(); ite != enemies.end();)
//		{
//			if (Distance(ite->first->GetPos(), this->GetPos()) < 2)
//			{				
//				isEnd = true;
//				if (ite->first->isDead == false)
//				{
//					ite->first->TakeHurt();
//				}				
//				ite++;
//			}
//			else
//				ite++;
//		}
//	}
//	else
//	{
//		if (player != nullptr)
//		{
//			if (Distance(player->GetPos(), this->GetPos()) < 2)
//			{
//				isEnd = true;				
//				player->TakeHurt();
//			}
//		}		
//	}
//}

