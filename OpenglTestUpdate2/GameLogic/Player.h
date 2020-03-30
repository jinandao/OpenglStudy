#pragma once
#include "../GameLayer/Avatar.h"

class Player :public Avatar
{
private:

public:
	Player(Vector3f _scale, Vector3f _rotation, Vector3f _worldPos, int _hp, float _speed, float _angleSpeed) :
		Avatar(_scale, _rotation, _worldPos, _hp, _speed, _angleSpeed)
	{}
	Player(const Player& player) = delete;
	Player& operator=(const Player& player) = delete;
	virtual void Update();
	virtual void Shoot(std::vector<Bullet*>& bullets);
	virtual void TakeHurt();
	void TakeAction(char c, std::vector<Bullet*>& bullets);
};