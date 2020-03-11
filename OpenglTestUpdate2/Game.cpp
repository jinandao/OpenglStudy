/*
对游戏中物体的封装
*/

#include "Game.h"
#include <iostream>

void RenderItem::Init()
{
}

void Avatar::TakeImage()
{
}

void Avatar::Shoot()
{
}

void Avatar::MoveForward()
{
	float angle = rotation.y;
	PositionChange(speed * cosf(ToRadian(angle + 90)), 0, speed * sinf(ToRadian(angle + 90)));
}

void Avatar::MoveBack()
{
	float angle = rotation.y;
	PositionChange(-speed * cosf(ToRadian(angle + 90)), 0, -speed * sinf(ToRadian(angle + 90)));
}

void Avatar::TurnRight()
{
	RotationChange(0, -angleSpeed, 0);
}

void Avatar::TurnLeft()
{
	RotationChange(0, angleSpeed, 0);
}

void Avatar::Die()
{
}

void Player::Update()
{
}

void Enemy::Update()
{
}

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
	float angle = rotation.y;
	PositionChange(speed * cosf(ToRadian(angle + 90)), 0, speed * sinf(ToRadian(angle + 90)));
	std::cout << "bullet move" << std::endl;
}

void Bullet::Destroy()
{
}
