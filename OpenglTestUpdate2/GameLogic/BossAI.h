#pragma once
#include "EnemyAI.h"

class Avatar;

class BossAI:EnemyAI
{
private:
	bool isInboss;//普通状态或者进入boss独特状态
	int collideTimes;//进入Boss状态后冲撞的次数
	int collideupdateTimes;
	int normalupdateTimes;

	bool Collide();
public:
	BossAI(Avatar *player,Enemy* _boss);
	~BossAI();
	virtual void Update(std::vector<Bullet*>& bullets);
	
};