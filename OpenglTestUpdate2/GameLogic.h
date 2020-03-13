/*
游戏中逻辑相关的代码文件
*/

#pragma once
#include "Game.h"
#include "Camera.h"

class EnemyAI
{
private:
	Avatar* player;
	Enemy* self;
	float timer;
	bool TurnToPlayer(float angle);
	
public:
	EnemyAI(Avatar* _player,Enemy* _self):player(_player),self(_self),timer(0)
	{}
	
	void Update(std::vector<Bullet*>& bullets);
};