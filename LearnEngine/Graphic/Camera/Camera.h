#pragma once

namespace LearnEngine {

class ConstBuffer;

class Camera {
public:
	Camera();
	~Camera();

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
		DirectX::XMMATRIX m = DirectX::XMMatrixLookAtLH(cameraPos, lookingPos, cameraUp);
		m = DirectX::XMMatrixTranspose(m);
		return m;
	}

	Matrix getProjMat() {
		DirectX::XMMATRIX m = DirectX::XMMatrixPerspectiveFovLH(m_fov, m_aspect, m_near, m_far);
		m = DirectX::XMMatrixTranspose(m);
		return m;
	}

	//現在の設定をシェーダーの定数バッファに適用する。
	void Apply();

private:
	Vector3 cameraPos;
	Vector3 lookingPos;
	Vector3 cameraUp;

	float m_far = 0;
	float m_near = 0;
	float m_aspect = 0;
	float m_fov = 0;
};

}