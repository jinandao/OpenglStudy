#pragma once
#include <algorithm>
#include "../GameLayer/RenderItem.h"

class Bullet :public RenderItem
{
private:
	std::shared_ptr<Geometry> geo;
	float speed;
	float timer;
	bool isPlayer;
public:
	bool isEnd;
public:
	Bullet(Vector3f _scale, Vector3f _rotation, Vector3f _pos, float _speed, bool _isPlayer) :
		RenderItem(_scale, _rotation, _pos), geo(nullptr), speed(_speed), timer(0), isPlayer(_isPlayer), isEnd(false)
	{}
	~Bullet(){}
	void Init();
	inline void SetGeo(std::shared_ptr<Geometry>& _geo);	
	inline std::shared_ptr<Geometry>& GetGeo();
	
	void Move();
	void CheckHurt();
};

inline void Bullet::SetGeo(std::shared_ptr<Geometry>& _geo)
{
	geo = _geo;
}

inline std::shared_ptr<Geometry>& Bullet::GetGeo()
{
	return geo;
}