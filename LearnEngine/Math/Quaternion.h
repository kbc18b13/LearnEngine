#pragma once
#include "Vector3.h"
#include "Pi.h"

namespace LearnEngine {

struct Quaternion {
	DirectX::XMFLOAT4 dxQu;

	Quaternion(){
		DirectX::XMStoreFloat4(&dxQu, DirectX::XMQuaternionIdentity());
	}
	Quaternion(const Quaternion& q) : dxQu(q.dxQu){}
	Quaternion(const DirectX::XMVECTOR& v) {
		DirectX::XMStoreFloat4(&dxQu, v);
	}

	operator DirectX::XMVECTOR() const {
		return DirectX::XMLoadFloat4(&dxQu);
	}

	//�N�H�[�^�j�I�����m������
	void multiply(const Quaternion& q) {
		*this = DirectX::XMQuaternionMultiply(*this, q);
	}
	
	//�x�N�g������]������
	void rotateVec(Vector3& vec) {
		vec = DirectX::XMVector3Rotate(vec, *this);
	}

	//���ʐ��`���
	Quaternion slerp(const Quaternion& q, float percent) {
		return DirectX::XMQuaternionSlerp(*this, q, percent);
	}

////////////////////////////�@��������static�֐��@/////////////////////////////////////

	//������]���Ȃ�Quaternion
	static Quaternion Identity() {
		return DirectX::XMQuaternionIdentity();
	}

	//���Ɗp�x(���W�A��)����擾�B
	static Quaternion GetRotation(const Vector3& axis, float radian) {
		return DirectX::XMQuaternionRotationAxis(axis, radian);
	}

	//���Ɗp�x(�x)����擾�B
	static Quaternion GetRotationDeg(const Vector3& axis, float deg) {
		return GetRotation(axis, DegToRad(deg));
	}

	//2�x�N�g���̓��ς���擾
	static Quaternion GetRotationDot(const Vector3& vec1, const Vector3& vec2) {
		return DirectX::XMQuaternionDot(vec1, vec2);
	}

	//�I�C���[�p����擾
	static Quaternion GetRotationEuler(float rall, float pitch, float yaw) {
		return DirectX::XMQuaternionRotationRollPitchYaw(pitch, yaw, rall);
	}
};

}