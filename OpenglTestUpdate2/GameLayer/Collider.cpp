#include "Collider.h"
#include <math.h>

BoxCollider::BoxCollider(float _x, float _y, float _z, float _width, float _length, float _height):
	x(_x),y(_y),z(_z),width(_width),length(_length),height(_height)
{}

bool BoxCollider::CheckIsIn(const float& _x, const float& _y, const float& _z) const
{
	float x1 = abs(x - _x);
	float y1 = abs(y - _y);
	float z1 = abs(z - _z);
	if (x1 > length / 2 && y1 > height / 2 && z1 > width / 2)
	{
		return true;
	}
	return false;
}

void BoxCollider::SetPos(float _x, float _y, float _z)
{
	x = _x;
	y = _y;
	z = _z;
}

void BoxCollider::SetSize(float _width, float _height, float _length)
{
	width = _width;
	height = _height;
	length = _length;
}

void BoxCollider::SetTag(const std::string& _tag)
{
	tag = _tag;
}

std::string BoxCollider::Tag()
{
	return tag;
}
