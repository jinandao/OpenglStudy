#pragma once
#include "../EngineLayer/MathTools.h"
#include "Camera.h"

class Pipeline
{
private:
	Vector3f m_scale;
	Vector3f m_worldPos;
	Vector3f m_rotateInfo;

	PersProjInfo m_persProjInfo;
	OrthoProjInfo m_orthoProjInfo;

	struct {
		Vector3f Pos;
		Vector3f Target;
		Vector3f Up;
	} m_camera;

	Matrix4f m_WVPtransformation;
	Matrix4f m_VPtransformation;
	Matrix4f m_WPtransformation;
	Matrix4f m_WVtransformation;
	Matrix4f m_Wtransformation;
	Matrix4f m_Vtransformation;
	Matrix4f m_ProjTransformation;

public:
	Pipeline()
	{
		m_scale = Vector3f(1.0f, 1.0f, 1.0f);
		m_worldPos = Vector3f(0.0f, 0.0f, 0.0f);
		m_rotateInfo = Vector3f(0.0f, 0.0f, 0.0f);
	}

	//设置scale
	void Scale(float s)
	{
		Scale(s, s, s);
	}

	void Scale(const Vector3f& scale)
	{
		Scale(scale.x, scale.y, scale.z);
	}

	void Scale(float ScaleX, float ScaleY, float ScaleZ)
	{
		m_scale.x = ScaleX;
		m_scale.y = ScaleY;
		m_scale.z = ScaleZ;
	}

	//设置position
	void WorldPos(float x, float y, float z)
	{
		m_worldPos.x = x;
		m_worldPos.y = y;
		m_worldPos.z = z;
	}

	void WorldPos(const Vector3f& Pos)
	{
		m_worldPos = Pos;
	}

	//设置rotation
	void Rotate(float RotateX, float RotateY, float RotateZ)
	{
		m_rotateInfo.x = RotateX;
		m_rotateInfo.y = RotateY;
		m_rotateInfo.z = RotateZ;
	}

	void Rotate(const Vector3f& r)
	{
		Rotate(r.x, r.y, r.z);
	}

	//设置投影矩阵
	void SetPerspectiveProj(const PersProjInfo& p)
	{
		m_persProjInfo = p;
	}

	//设置正交投影矩阵
	void SetOrthographicProj(const OrthoProjInfo& p)
	{
		m_orthoProjInfo = p;
	}

	//设置相机
	void SetCamera(const Vector3f& Pos, const Vector3f& Target, const Vector3f& Up)
	{
		m_camera.Pos = Pos;
		m_camera.Target = Target;
		m_camera.Up = Up;
	}

	void SetCamera(const Camera& camera)
	{
		SetCamera(camera.GetPos(), camera.GetTarget(), camera.GetUp());
	}

	void Orient(const Orientation& o)
	{
		m_scale = o.scale;
		m_worldPos = o.pos;
		m_rotateInfo = o.rotation;
	}

	void SetObjectInfo(const Vector3f& pos,const Vector3f& scale,const Vector3f& rotation)
	{
		Scale(scale);
		WorldPos(pos);
		Rotate(rotation);
	}

	const Matrix4f& GetWPTrans();
	const Matrix4f& GetWVTrans();
	const Matrix4f& GetVPTrans();
	const Matrix4f& GetWVPTrans();
	const Matrix4f& GetWVOrthoPTrans();
	const Matrix4f& GetWorldTrans();
	const Matrix4f& GetViewTrans();
	const Matrix4f& GetProjTrans();
};

