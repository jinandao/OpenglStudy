/*
相机相关代码文件
*/

#pragma once
#include "../EngineLayer/MathTools.h"

class Camera
{
public:
	void Init();
	void Update();

	Vector3f pos;
	Vector3f target;
	Vector3f up;

	int windowWidth;
	int windowHeight;
	
	float angleH;
	float angleV;

	bool onUpperEdge;
	bool onLowerEdge;
	bool onLeftEdge;
	bool onRightEdge;

	Vector2i mousePos;

public:
	Camera(int _windowWidth, int _windowHeight);

	Camera(int _windowWidth, int _windowHeight, const Vector3f& Pos, const Vector3f& Target, const const Vector3f& Up);

	const Vector3f& GetPos() const
	{
		return pos;
	}

	const Vector3f& GetTarget() const
	{
		return target;
	}

	const Vector3f& GetUp() const
	{
		return up;
	}

	void SetTarget(const Vector3f& _target)
	{
		target = _target;
	}

	void SetPos(Vector3f _pos)
	{
		pos.x = _pos.x;
		pos.y = _pos.y;
		pos.z = _pos.z;
	}

	void OnMouse(int x, int y);

	void OnRender();

};

