/*
对游戏中物体与物体涉及的游戏逻辑的封装
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
	virtual ~RenderItem() {};
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

class Bullet:public RenderItem
{
private:
	std::shared_ptr<Geometry> geo;
	float speed;	
	float timer;
	bool isPlayer;
public:
	bool isEnd;
public:
	Bullet(Vector3f _scale,Vector3f _rotation,Vector3f _pos,float _speed,bool _isPlayer):
		RenderItem(_scale,_rotation,_pos),geo(nullptr),speed(_speed),timer(0),isPlayer(_isPlayer), isEnd(false)
	{}
	~Bullet()
	{
	}
	void Init();
	void SetGeo(std::shared_ptr<Geometry>& _geo)
	{
		geo = _geo;
	}
	std::shared_ptr<Geometry>& GetGeo()
	{		
		return geo;
	}
	void Move();
	void CheckHurt();
};

class Avatar:public RenderItem
{
protected:	
	int hp;
	float speed;
	float angleSpeed;
	std::shared_ptr<Mesh> pMesh;//多个avatar可能共用一个mesh
public:
	bool isDead;
public:
	Avatar() {}
	Avatar(Vector3f _scale, Vector3f _rotation, Vector3f _worldPos, int _hp, float _speed,float _angleSpeed):
		 RenderItem(_scale,_rotation,_worldPos),hp(_hp), speed(_speed),angleSpeed(_angleSpeed),pMesh(nullptr),isDead(false)
	{}
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
	virtual void TakeHurt();
	virtual void Shoot(std::vector<Bullet*>& bullets)=0;
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
	Player(Vector3f _scale, Vector3f _rotation, Vector3f _worldPos,int _hp,float _speed,float _angleSpeed):
		Avatar(_scale,_rotation,_worldPos,_hp,_speed,_angleSpeed)
	{}
	Player(const Player& player) = delete;
	Player& operator=(const Player& player) = delete;
	virtual void Update();
	virtual void Shoot(std::vector<Bullet*>& bullets);
	virtual void TakeHurt();
	void TakeAction(char c,std::vector<Bullet*>& bullets);
};

class Enemy :public Avatar
{
private:

public:
	Enemy(Vector3f _scale, Vector3f _rotation, Vector3f _worldPos, int _hp, float _speed,float _angleSpeed) :
		Avatar(_scale,_rotation,_worldPos,_hp,_speed,_angleSpeed)
	{}
	Enemy(const Enemy& enemy) = delete;
	Enemy& operator=(const Enemy& enemy) = delete;
	virtual void Update();
	virtual void Shoot(std::vector<Bullet*>& bullets);
	virtual void TakeHurt();
};

void Destroy(RenderItem* item);