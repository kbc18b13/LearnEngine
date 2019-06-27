#pragma once
namespace LearnEngine {

class Camera {
public:
	void setLook(const DirectX::XMFLOAT3& look) {
		lookingPos = look;
	}
	void setPos(const DirectX::XMFLOAT3& pos) {
		cameraPos = pos;
	}
	void setUp(const DirectX::XMFLOAT3& up) {
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
	void setFOV(float f) {
		m_fov = f;
	}

	DirectX::XMMATRIX getViewMat() {
		return DirectX::XMMatrixLookAtLH(DirectX::XMLoadFloat3(&cameraPos),
										 DirectX::XMLoadFloat3(&lookingPos),
										 DirectX::XMLoadFloat3(&cameraUp));
	}

	DirectX::XMMATRIX getProjMat() {
		return DirectX::XMMatrixPerspectiveFovLH(m_fov, m_aspect, m_near, m_far);
	}

private:
	DirectX::XMFLOAT3 cameraPos;
	DirectX::XMFLOAT3 lookingPos;
	DirectX::XMFLOAT3 cameraUp;

	float m_far;
	float m_near;
	float m_aspect;
	float m_fov;
};

}