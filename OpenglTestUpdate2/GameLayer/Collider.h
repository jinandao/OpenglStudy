#pragma once
#include <string>

class BoxCollider
{
private:
	float x;
	float y;
	float z;
	float width;
	float length;
	float height;
	std::string tag;

public:
	BoxCollider(float _x, float _y, float _z, float _width, float _length, float _height);
	bool CheckIsIn(const float &_x,const float &_y,const float &_z) const;
	void SetPos(float _x, float _y, float _z);
	void SetSize(float _width, float _height, float _length);
	void SetTag(const std::string& _tag);
	std::string Tag();
};