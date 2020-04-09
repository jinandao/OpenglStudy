#include "Enemy.h"
#include "Bullet.h"
#include "../GameLayer/ParticleSystem.h"

extern std::vector<ParticleElement> particles;

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

void Enemy::Die()
{
	//ParticleSystem* particle = new ParticleSystem();
	ParticleElement pe;
	pe.times = 180;
	pe.pos = GetPos();
	pe.ps = nullptr;
	particles.push_back(pe);

	Avatar::Die();
}

//void Enemy::Update(){}