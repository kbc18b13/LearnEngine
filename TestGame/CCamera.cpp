#include "stdafx.h"
#include "CCamera.h"

bool CCamera::Start() {
	LE::Camera& c = LE::MainCamera();
	c.setFar(300.0f);
	c.setNear(1.0f);
	c.setAspect(16.0f / 9);
	c.setFOV(LE::DegToRad(90.0f));

	c.setLook({ 0, 0, 0 });
	c.setPos(pos);
	c.setUp(up);
	return true;
}

void CCamera::Update() {
	LE::Quaternion q;
	{
		using namespace LE;
		const Vector3 eyeline = Vector3(0, 0, 0) - pos;
		Vector3 axis = up.cross(eyeline);

		if (GetAsyncKeyState('W')) {
			q.multiply(Quaternion::GetRotationDeg(axis, 0.2f));
		}
		if (GetAsyncKeyState('S')) {
			q.multiply(Quaternion::GetRotationDeg(axis, -0.2f));
		}
		if (GetAsyncKeyState('A')) {
			q.multiply(Quaternion::GetRotationDeg(Vector3::AxisY(), 0.2f));
		}
		if (GetAsyncKeyState('D')) {
			q.multiply(Quaternion::GetRotationDeg(Vector3::AxisY(), -0.2f));
		}
		q.rotateVec(up);
		q.rotateVec(pos);
	}

	LE::Camera& c = LE::MainCamera();
	c.setPos(pos);
	c.setUp(up);
}
