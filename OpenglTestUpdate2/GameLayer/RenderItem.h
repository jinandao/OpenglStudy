#pragma once
#include "../EngineLayer/mesh.h"

class RenderItem
{
protected:
	Vector3f scale;
	Vector3f rotation;
	Vector3f worldPos;

public:
	RenderItem() {}
	RenderItem(Vector3f _scale, Vector3f _rotation, Vector3f _worldPos) :
		scale(_scale), rotation(_rotation), worldPos(_worldPos)
	{}
	virtual ~RenderItem() {};
	Vector3f GetPos() { return worldPos; }
	Vector3f GetScale() { return scale; }
	Vector3f GetRotation() { return rotation; }

	inline void PositionChange(float x, float y, float z);
	inline void RotationChange(float x, float y, float z);
};

inline void RenderItem::PositionChange(float x, float y, float z)
{
	worldPos.x += x;
	worldPos.y += y;
	worldPos.z += z;
}

inline void RenderItem::RotationChange(float x, float y, float z)
{
	rotation.x += x;
	rotation.y += y;
	rotation.z += z;
}