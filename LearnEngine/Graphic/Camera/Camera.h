#pragma once
namespace LearnEngine {

class Camera {
public:
	void setLook(const Vector3& look) {
		lookingPos = look;
	}
	void setPos(const Vector3& pos) {
		cameraPos = pos;
	}
	void setUp(const Vector3& up) {
		cameraUp = up;
	}
	void setFar(float f) {
		m_far = f;
	}
	void setNear(float n) {
		m_near = n;
	}
	void setAspect(float a) {
		m_aspect = a;
	}
	void setFOV(float radian) {
		m_fov = radian;
	}

	Matrix getViewMat() {
		return DirectX::XMMatrixLookAtLH(cameraPos, lookingPos, cameraUp);
	}

	Matrix getProjMat() {
		return DirectX::XMMatrixPerspectiveFovLH(m_fov, m_aspect, m_near, m_far);
	}

private:
	Vector3 cameraPos;
	Vector3 lookingPos;
	Vector3 cameraUp;

	float m_far;
	float m_near;
	float m_aspect;
	float m_fov;
};

}