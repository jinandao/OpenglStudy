/*
对游戏中物体的封装
*/

#pragma once
#include "OpenglTools.h"
#include "mesh.h"
#include <algorithm>

class RenderItem
{
protected:
	Vector3f scale;
	Vector3f rotation;
	Vector3f worldPos;

public:
	RenderItem(){}
	RenderItem(Vector3f _scale,Vector3f _rotation, Vector3f _worldPos):
		scale(_scale),rotation(_rotation),worldPos(_worldPos)
	{}
	void Init();
	Vector3f GetPos() { return worldPos; }
	Vector3f GetScale() { return scale; }
	Vector3f GetRotation() { return rotation; }

	void PositionChange(float x, float y, float z) 
	{
		worldPos.x += x;
		worldPos.y += y;
		worldPos.z += z;
	}
	void RotationChange(float x, float y, float z)
	{
		rotation.x += x;
		rotation.y += y;
		rotation.z += z;
	}
};

//架构需要修改
class Bullet:public RenderItem
{
private:
	std::shared_ptr<Geometry> geo;
	float speed;	
public:
	Bullet(Vector3f _scale,Vector3f _rotation,Vector3f _pos,float _speed):RenderItem(_scale,_rotation,_pos),geo(nullptr),speed(_speed)
	{}
	~Bullet()
	{}
	void Init();
	std::shared_ptr<Geometry>& GetGeo()
	{		
		return geo;
	}
	void Move();
	void Destroy();
};

class Avatar:public RenderItem
{
private:	
	int hp;
	int attack;
	float speed;
	float angleSpeed;
	std::shared_ptr<Mesh> pMesh;//多个avatar可能共用一个mesh
public:
	Avatar() {}
	Avatar(Vector3f _scale, Vector3f _rotation, Vector3f _worldPos, int _hp, int _attack, float _speed,float _angleSpeed):
		 RenderItem(_scale,_rotation,_worldPos),hp(_hp), attack(_attack), speed(_speed),angleSpeed(_angleSpeed),pMesh(nullptr)
	{}
	~Avatar()
	{	}
	Avatar(const Avatar& avatar) = delete;
	Avatar& operator=(const Avatar& avtar) = delete;
	
	void SetMesh(const std::string& Filename)
	{		
		pMesh = std::shared_ptr<Mesh>(new Mesh());
		pMesh->LoadMesh(Filename);		
	}
	//Mesh* GetMesh() { return pMesh; }
	void SetMesh(const std::shared_ptr<Mesh> mesh)
	{
		pMesh = mesh;
	}
	virtual void TakeImage();
	virtual void Shoot();
	virtual void MoveForward();
	virtual void MoveBack();
	virtual void TurnRight();
	virtual void TurnLeft();
	virtual void Die();
	virtual void Update()=0;
	void Render() 
	{
		pMesh->Render();
	}
	std::shared_ptr<Mesh>& GetMesh()
	{
		return pMesh;
	}
};

class Player :public Avatar
{
private:
	
public:
	Player(Vector3f _scale, Vector3f _rotation, Vector3f _worldPos,int _hp,int _attack,float _speed,float _angleSpeed):
		Avatar(_scale,_rotation,_worldPos,_hp,_attack,_speed,_angleSpeed)
	{}
	Player(const Player& player) = delete;
	Player& operator=(const Player& player) = delete;
	virtual void Update();
};

class Enemy :public Avatar
{
private:
	int hp;
	int attack;
	float speed;
public:
	Enemy(Vector3f _scale, Vector3f _rotation, Vector3f _worldPos, int _hp, int _attack, float _speed,float _angleSpeed) :
		Avatar(_scale, _rotation, _worldPos, _hp, _attack, _speed,_angleSpeed)
	{}
	Enemy(const Enemy& enemy) = delete;
	Enemy& operator=(const Enemy& enemy) = delete;
	virtual void Update();
};