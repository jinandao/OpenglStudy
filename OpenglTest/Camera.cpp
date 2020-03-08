#include "Camera.h"

const static int MARGIN = 10;
const static float EDGE_STEP = 0.5f;

void Camera::Init()
{
	Vector3f HTarget(target.x, 0.0, target.z);
	HTarget.Normalize();

	if (HTarget.z >= 0.0f)
	{
		if (HTarget.x >= 0.0f)
		{
			angleH = 360.0f - ToDegree(asin(HTarget.z));
		}
		else
		{
			angleH = 180.0f + ToDegree(asin(HTarget.z));
		}
	}
	else
	{
		if (HTarget.x >= 0.0f)
		{
			angleH = ToDegree(asin(-HTarget.z));
		}
		else
		{
			angleH = 180.0f - ToDegree(asin(-HTarget.z));
		}
	}

	angleV= -ToDegree(asin(target.y));

	onUpperEdge = false;
	onLowerEdge = false;
	onLeftEdge = false;
	onRightEdge = false;

	mousePos.x = windowWidth / 2;
	mousePos.y = windowHeight / 2;
}

void Camera::Update()
{
	const Vector3f Vaxis(0.0f, 1.0f, 0.0f);

	// Rotate the view vector by the horizontal angle around the vertical axis
	Vector3f View(1.0f, 0.0f, 0.0f);
	View.Rotate(angleH, Vaxis);
	View.Normalize();

	// Rotate the view vector by the vertical angle around the horizontal axis
	Vector3f Haxis = Vaxis.Cross(View);
	Haxis.Normalize();
	View.Rotate(angleV, Haxis);

	target = View;
	target.Normalize();

	up = target.Cross(Haxis);
	up.Normalize();
}

Camera::Camera(int _windowWidth, int _windowHeight):windowWidth(_windowWidth),windowHeight(_windowHeight),
pos(Vector3f(0.0f,0.0f,0.0f)),target(Vector3f(0.0f,0.0f,1.0f)),up(0.0f,1.0f,0.0f)
{
	Init();
}

Camera::Camera(int _windowWidth, int _windowHeight, const Vector3f& _pos, const Vector3f& _target, const const Vector3f& _up):
windowWidth(_windowWidth),windowHeight(_windowHeight),pos(_pos),target(_target),up(_up)
{
	Init();
}

void Camera::OnMouse(int x, int y)
{
	const int DeltaX = x - mousePos.x;
	const int DeltaY = y - mousePos.y;

	mousePos.x = x;
	mousePos.y = y;

	angleH += (float)DeltaX / 20.0f;
	angleV += (float)DeltaY / 20.0f;

	if (DeltaX == 0) {
		if (x <= MARGIN) {
			//    m_AngleH -= 1.0f;
			onLeftEdge = true;
		}
		else if (x >= (windowWidth - MARGIN)) {
			//    m_AngleH += 1.0f;
			onRightEdge = true;
		}
	}
	else {
		onLeftEdge = false;
		onRightEdge = false;
	}

	if (DeltaY == 0) {
		if (y <= MARGIN) {
			onUpperEdge = true;
		}
		else if (y >= (windowHeight - MARGIN)) {
			onLowerEdge = true;
		}
	}
	else {
		onUpperEdge = false;
		onLowerEdge = false;
	}

	Update();
}

void Camera::OnRender()
{
	bool ShouldUpdate = false;

	if (onLeftEdge) {
		angleH -= EDGE_STEP;
		ShouldUpdate = true;
	}
	else if (onRightEdge) {
		angleH += EDGE_STEP;
		ShouldUpdate = true;
	}

	if (onUpperEdge) {
		if (angleV > -90.0f) {
			angleV -= EDGE_STEP;
			ShouldUpdate = true;
		}
	}
	else if (onLowerEdge) {
		if (angleV < 90.0f) {
			angleV += EDGE_STEP;
			ShouldUpdate = true;
		}
	}

	if (ShouldUpdate) {
		Update();
	}
}
