#include "Enemy.h"
#include "Bullet.h"

void Enemy::Shoot(std::vector<Bullet*>& bullets)
{
	Bullet* bullet = new Bullet(Vector3f(2, 2, 2), GetRotation(), GetPos(), 1, false);
	if (bullets.size() == 0)
	{
		bullet->Init();
	}
	else
	{
		if (bullets[0] == nullptr)
			bullet->Init();
		else
			bullet->SetGeo(bullets[0]->GetGeo());
	}
	bullets.push_back(bullet);
}

void Enemy::TakeHurt()
{
	Avatar::TakeHurt();
	if (hp <= 0)
	{
		Die();
	}
}

void Enemy::Update(){}