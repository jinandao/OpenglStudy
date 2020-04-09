#pragma once
#include "../GameLayer/Avatar.h"

class Enemy :public Avatar
{
private:

public:
	Enemy(Vector3f _scale, Vector3f _rotation, Vector3f _worldPos, int _hp, float _speed, float _angleSpeed) :
		Avatar(_scale, _rotation, _worldPos, _hp, _speed, _angleSpeed)
	{}
	Enemy(const Enemy& enemy) = delete;
	Enemy& operator=(const Enemy& enemy) = delete;
	//virtual void Update();
	virtual void Shoot(std::vector<Bullet*>& bullets);
	virtual void TakeHurt();
	virtual void Die();
};