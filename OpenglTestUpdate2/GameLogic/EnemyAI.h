#pragma once
#include <vector>

class Avatar;
class Enemy;
class Bullet;

class EnemyAI
{
private:
	Avatar* player;
	Enemy* self;
	float timer;
	bool TurnToPlayer(float angle);

public:
	EnemyAI(Avatar* _player, Enemy* _self) :player(_player), self(_self), timer(0)
	{}

	void Update(std::vector<Bullet*>& bullets);
};