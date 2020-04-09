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
		if (collideTimes < 3)//��ײ����С��3ʱ���嵽ĳһ���ת�򣬼�����ײ
		{
			if (!Collide())//����ĳһ���!colliderΪtrue
			{				
				if (temp < 60)//ת������60��ѭ��
				{
					temp += 1;
					EnemyAI::Update(bullets);//ʹ�û��ຯ��ʹBoss��ײ��ת��
				}
				else//ת����������ò��ֲ���
				{
					temp = 0;
					collideTimes += 1;
					collideupdateTimes = 40;
				}
			}
		}
		else//�����3�κ󣬽�����ͨ��׷�١����״̬
		{
			collideTimes = 0;
			isInboss = false;
		}
	}
	else//��ͨ��׷�����״̬ѭ����ٴκ��ٴν�����״̬
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
