#pragma once
#include <vector>

class Avatar;
class Enemy;
class Bullet;

class EnemyAI
{
protected:
	Avatar* player;
	Enemy* self;
	float timer;
	bool TurnToPlayer(float angle);

public:
	EnemyAI(Avatar* _player, Enemy* _self) :player(_player), self(_self), timer(0)
	{}

	virtual void Update(std::vector<Bullet*>& bullets);
};