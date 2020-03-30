#include "Avatar.h"

void Avatar::TakeHurt()
{
	hp -= 10;
}

void Avatar::MoveForward()
{
	float angle = rotation.y;
	PositionChange(speed * cosf(ToRadian(angle + 90)), 0, speed * sinf(ToRadian(angle + 90)));
}

void Avatar::MoveBack()
{
	float angle = rotation.y;
	PositionChange(-speed * cosf(ToRadian(angle + 90)), 0, -speed * sinf(ToRadian(angle + 90)));
}

void Avatar::TurnRight()
{
	RotationChange(0, -angleSpeed, 0);
}

void Avatar::TurnLeft()
{
	RotationChange(0, angleSpeed, 0);
}

void Avatar::Die()
{
	isDead = true;
}