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

	//クォータニオン同士を合成
	void multiply(const Quaternion& q) {
		*this = DirectX::XMQuaternionMultiply(*this, q);
	}
	
	//ベクトルを回転させる
	void rotateVec(Vector3& vec) {
		vec = DirectX::XMVector3Rotate(vec, *this);
	}

	//球面線形補間
	Quaternion slerp(const Quaternion& q, float percent) {
		return DirectX::XMQuaternionSlerp(*this, q, percent);
	}

////////////////////////////　ここからstatic関数　/////////////////////////////////////

	//何も回転しないQuaternion
	static Quaternion Identity() {
		return DirectX::XMQuaternionIdentity();
	}

	//軸と角度(ラジアン)から取得。
	static Quaternion GetRotation(const Vector3& axis, float radian) {
		return DirectX::XMQuaternionRotationAxis(axis, radian);
	}

	//軸と角度(度)から取得。
	static Quaternion GetRotationDeg(const Vector3& axis, float deg) {
		return GetRotation(axis, DegToRad(deg));
	}

	//2ベクトルの内積から取得
	static Quaternion GetRotationDot(const Vector3& vec1, const Vector3& vec2) {
		return DirectX::XMQuaternionDot(vec1, vec2);
	}

	//オイラー角から取得
	static Quaternion GetRotationEuler(float rall, float pitch, float yaw) {
		return DirectX::XMQuaternionRotationRollPitchYaw(pitch, yaw, rall);
	}
};

}