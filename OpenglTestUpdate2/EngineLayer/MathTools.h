/*
��ѧ��ش����ļ�
*/

#pragma once
#include <iostream>

#define M_PI 3.1415926

float ToRadian(float x);

float ToDegree(float x);

struct Vector2i
{
	int x;
	int y;
};

struct Vector2f
{
	float x;
	float y;
	Vector2f()
	{}

	Vector2f(float _x, float _y):x(_x),y(_y)
	{}
};

struct Vector3f
{
	float x;
	float y;
	float z;

	Vector3f(){}
	Vector3f(float _x, float _y, float _z):x(_x),y(_y),z(_z)
	{}
	
	Vector3f(const float* pFloat)//����õ�����
	{
		x = pFloat[0];
		y = pFloat[0];
		z = pFloat[0];
	}

	Vector3f(float f):x(f),y(f),z(f)
	{}

	Vector3f& operator=(const Vector3f& r)
	{
		x = r.x;
		y = r.y;
		z = r.z;
		return *this;
	}

	Vector3f& operator+=(const Vector3f& r)
	{
		x += r.x;
		y += r.y;
		z += r.z;

		return *this;
	}

	Vector3f& operator-=(const Vector3f& r)
	{
		x -= r.x;
		y -= r.y;
		z -= r.z;
		
		return *this;
	}

	Vector3f& operator*=(float f)
	{
		x *= f;
		y *= f;
		z *= f;
		
		return *this;
	}

	Vector3f& Normalize();

	Vector3f Cross(const Vector3f& v) const;

	void Rotate(float Angle, const Vector3f& Axis);

	void Print() const
	{
		std::cout << "x:" << x << " y:" << y << " z:" << z << std::endl;
	}

};

struct Vector4f
{
	float x;
	float y;
	float z;
	float w;

	Vector4f()
	{}

	Vector4f(float _x, float _y, float _z, float _w):x(_x),y(_y),z(_z),w(_w)
	{}

	void Print(bool endl = true) const
	{		
		std::cout << "x:" << x << " y:" << y << " z:" << z <<" w:"<<w<< std::endl;
	}

	Vector3f to3f() const
	{
		Vector3f v(x, y, z);
		return v;
	}
};

inline Vector3f operator+(const Vector3f& l, const Vector3f& r)
{
	return Vector3f(l.x + r.x, l.y + r.y, l.z + r.z);
}

inline Vector3f operator-(const Vector3f& l, const Vector3f& r)
{
	return Vector3f(l.x - r.x, l.y - r.y, l.z - r.z);
}

inline Vector3f operator*(const Vector3f& l, float f)
{
	return Vector3f(l.x * f, l.y * f, l.z * f);
}

struct Quaternion
{
	float x, y, z, w;

	Quaternion(float _x, float _y, float _z, float _w);
	
	void Normalize();
	
	Quaternion Conjugate();
	
	Vector3f ToDegrees();
};

Quaternion operator*(const Quaternion& l, const Quaternion& r);

Quaternion operator*(const Quaternion& q, const Vector3f& v);

struct PersProjInfo
{
	float FOV;
	float Width;
	float Height;
	float zNear;
	float zFar;
};

struct OrthoProjInfo
{
	float r;        // right
	float l;        // left
	float b;        // bottom
	float t;        // top
	float n;        // z near
	float f;        // z far
};

class Matrix4f
{
public:
	float m[4][4];
	
	Matrix4f()
	{}

	Matrix4f(float a00, float a01, float a02, float a03,
		float a10, float a11, float a12, float a13,
		float a20, float a21, float a22, float a23,
		float a30, float a31, float a32, float a33)
	{
		m[0][0] = a00; m[0][1] = a01; m[0][2] = a02; m[0][3] = a03;
		m[1][0] = a10; m[1][1] = a11; m[1][2] = a12; m[1][3] = a13;
		m[2][0] = a20; m[2][1] = a21; m[2][2] = a22; m[2][3] = a23;
		m[3][0] = a30; m[3][1] = a31; m[3][2] = a32; m[3][3] = a33;
	}
	
	
	Matrix4f Transpose() const
	{
		Matrix4f n;

		for (unsigned int i = 0; i < 4; i++) {
			for (unsigned int j = 0; j < 4; j++) {
				n.m[i][j] = m[j][i];
			}
		}

		return n;
	}

	void InitIdentity()
	{
		m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = 0.0f;
		m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = 0.0f;
		m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = 0.0f;
		m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
	}

	Matrix4f operator*(const Matrix4f& Right) const
	{
		Matrix4f Ret;

		for (unsigned int i = 0; i < 4; i++) {
			for (unsigned int j = 0; j < 4; j++) {
				Ret.m[i][j] = m[i][0] * Right.m[0][j] +
					m[i][1] * Right.m[1][j] +
					m[i][2] * Right.m[2][j] +
					m[i][3] * Right.m[3][j];
			}
		}

		return Ret;
	}

	Vector4f operator*(const Vector4f& v) const
	{
		Vector4f r;

		r.x = m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3] * v.w;
		r.y = m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3] * v.w;
		r.z = m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3] * v.w;
		r.w = m[3][0] * v.x + m[3][1] * v.y + m[3][2] * v.z + m[3][3] * v.w;

		return r;
	}

	operator const float* () const
	{
		return &(m[0][0]);
	}

	float Determinant() const;

	Matrix4f& Inverse();

	void InitScaleTransform(float ScaleX, float ScaleY, float ScaleZ);
	void InitRotateTransform(float RotateX, float RotateY, float RotateZ);
	void InitRotateTransform(const Quaternion& quat);
	void InitTranslationTransform(float x, float y, float z);
	void InitCameraTransform(const Vector3f& Target, const Vector3f& Up);

	void InitPersProjTransform(const PersProjInfo& p);
	void InitOrthoProjTransform(const OrthoProjInfo& p);	
};

struct Orientation
{
	Vector3f scale;
	Vector3f rotation;
	Vector3f pos;

	Orientation():scale(Vector3f(1.0f,1.0f,1.0f)),rotation(Vector3f(0.0f,0.0f,0.0f)),pos(Vector3f(0.0f,0.0f,0.0f))
	{}
};

float Distance(Vector3f a, Vector3f b);