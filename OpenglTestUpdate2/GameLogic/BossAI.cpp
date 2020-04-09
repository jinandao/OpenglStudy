#include "BossAI.h"
#include "../GameLayer/Avatar.h"
#include "Enemy.h"

int temp = 0;

BossAI::BossAI(Avatar *player,Enemy* _boss):EnemyAI(player, _boss),isInboss(true), collideTimes(0), collideupdateTimes(40), normalupdateTimes(0)
{}

BossAI::~BossAI()
{
}

void BossAI::Update(std::vector<Bullet*>& bullets)
{
	if (isInboss)
	{
		if (collideTimes < 3)//冲撞次数小于3时，冲到某一点后转向，继续冲撞
		{
			if (!Collide())//到达某一点后，!collider为true
			{				
				if (temp < 60)//转向消耗60次循环
				{
					temp += 1;
					EnemyAI::Update(bullets);//使用基类函数使Boss冲撞后转向
				}
				else//转向结束，重置部分参数
				{
					temp = 0;
					collideTimes += 1;
					collideupdateTimes = 40;
				}
			}
		}
		else//冲击完3次后，进入普通的追踪、射击状态
		{
			collideTimes = 0;
			isInboss = false;
		}
	}
	else//普通的追踪射击状态循环五百次后，再次进入冲击状态
	{
		if (normalupdateTimes < 400)
		{
			normalupdateTimes += 1;
			EnemyAI::Update(bullets);
		}
		else
		{
			normalupdateTimes = 0;
			isInboss = true;
		}
	}
}

bool BossAI::Collide()
{
	//std::cout << "test" << std::endl;
	if (collideupdateTimes > 0)
	{
		collideupdateTimes -= 1;
		self->MoveForward();
		return true;
	}
	else
	{		
		return false;
	}
}
