#pragma once
#include "EnemyAI.h"

class Avatar;

class BossAI:EnemyAI
{
private:
	bool isInboss;//��ͨ״̬���߽���boss����״̬
	int collideTimes;//����Boss״̬���ײ�Ĵ���
	int collideupdateTimes;
	int normalupdateTimes;

	bool Collide();
public:
	BossAI(Avatar *player,Enemy* _boss);
	~BossAI();
	virtual void Update(std::vector<Bullet*>& bullets);
	
};