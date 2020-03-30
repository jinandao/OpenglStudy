#include "Player.h"
#include "Bullet.h"

void Player::Update()
{

}

void Player::Shoot(std::vector<Bullet*>& bullets)
{
	Bullet* bullet = new Bullet(Vector3f(2, 2, 2), GetRotation(), GetPos(), 3, true);
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

void Player::TakeHurt()
{
	Avatar::TakeHurt();
	if (hp <= 0)
	{
		Die();
	}
}

void Player::TakeAction(char c, std::vector<Bullet*>& bullets)
{
	if (c == 'w')
	{
		MoveForward();
	}
	if (c == 's')
	{
		MoveBack();
	}
	if (c == 'a')
	{
		TurnLeft();
	}
	if (c == 'd')
	{
		TurnRight();
	}
	if (c == 'j')
	{
		Shoot(bullets);
	}
}