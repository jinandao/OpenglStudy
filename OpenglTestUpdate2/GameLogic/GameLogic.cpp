#include "GameLogic.h"
/*
��Ϸ���߼���صĴ����ļ�
*/
//#include "../EngineLayer/MathTools.h"
//#include "OpenglTools.h"

//float temp;
//
//bool EnemyAI::TurnToPlayer(float angle)
//{
//	if (angle >= -20 && angle <= 20)
//	{
//		return true;
//	}
//	if (angle <-20 )
//	{
//		self->TurnRight();
//	}
//	else if(angle>20)
//	{
//		self->TurnLeft();
//	}
//	return false;
//}
//
//void EnemyAI::Update(std::vector<Bullet*>& bullets)
//{
//	float m = player->GetPos().x - self->GetPos().x;
//	float n = player->GetPos().z - self->GetPos().z;
//	float angle1 = ToDegree(atan2(n, m));
//	float angle = angle1 - self->GetRotation().y - 90;
//	if (angle > 360||angle<-360)
//	{
//		int k = angle / 360;
//		angle -= k * 360;
//	}	
//	//�Ծ���Ĵ���
//	if(Distance(player->GetPos(),self->GetPos())>100)
//	{ 
//		//�����Զ��ʲô������		
//	}
//	else if ((Distance(player->GetPos(), self->GetPos()) <= 100) && (Distance(player->GetPos(), self->GetPos()) > 25))
//	{
//		
//		if(TurnToPlayer(angle))
//			self->MoveForward();
//	}
//	else
//	{
//		if (TurnToPlayer(angle))
//		{
//			timer += 0.01;
//			if (timer > 1)
//			{
//				timer = 0;
//				self->Shoot(bullets);
//			}
//		}			
//	}	
//}
