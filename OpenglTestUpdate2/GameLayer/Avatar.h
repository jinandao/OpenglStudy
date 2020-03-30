#pragma once
#include "RenderItem.h"

class Bullet;

class Avatar :public RenderItem
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
	Avatar(Vector3f _scale, Vector3f _rotation, Vector3f _worldPos, int _hp, float _speed, float _angleSpeed) :
		RenderItem(_scale, _rotation, _worldPos), hp(_hp), speed(_speed), angleSpeed(_angleSpeed), pMesh(nullptr), isDead(false)
	{}
	Avatar(const Avatar& avatar) = delete;
	Avatar& operator=(const Avatar& avtar) = delete;

   	inline void SetMesh(const std::string& Filename);	
	inline void SetMesh(const std::shared_ptr<Mesh> mesh);		
	virtual void TakeHurt();
	virtual void Shoot(std::vector<Bullet*>& bullets) = 0;
	virtual void MoveForward();
	virtual void MoveBack();
	virtual void TurnRight();
	virtual void TurnLeft();
	virtual void Die();
	virtual void Update() = 0;
	inline void Render();	
	inline std::shared_ptr<Mesh>& GetMesh();
};

inline void Avatar::SetMesh(const std::string& Filename)
{
	pMesh = std::shared_ptr<Mesh>(new Mesh());
	pMesh->LoadMesh(Filename);
}

inline void Avatar::SetMesh(const std::shared_ptr<Mesh> mesh)
{
	pMesh = mesh;
}

inline void Avatar::Render()
{
	pMesh->Render();
}

inline std::shared_ptr<Mesh>& Avatar::GetMesh()
{
	return pMesh;
}