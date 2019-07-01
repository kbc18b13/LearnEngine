#pragma once

namespace LearnEngine {

union Vector3 {
	DirectX::XMFLOAT3 dxVec;
	struct {
		float x;
		float y;
		float z;
	};

	Vector3() : x(0), y(0), z(0) {}

	Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

	Vector3(const Vector3& v) : dxVec(v.dxVec) {}

	Vector3(const DirectX::XMVECTOR& v) {
		DirectX::XMStoreFloat3(&dxVec, v);
	}

	operator DirectX::XMVECTOR() {
		return DirectX::XMLoadFloat3(&dxVec);
	}

	Vector3 operator+(const Vector3& v) const{
		return Vector3(x + v.x, y + v.y, z + v.z);
	}

	Vector3 operator-(const Vector3& v)  const {
		return Vector3(x - v.x,  y - v.y,  z - v.z);
	}

	Vector3 operator*(float mul)  const {
		return Vector3(x * mul, y * mul, z * mul);
	}

	Vector3 operator/(float dev)  const {
		return Vector3(x/dev, y/dev, z/dev);
	}

	Vector3& operator+=(const Vector3& v){
		x += v.x, y += v.y, z += v.z;
		return *this;
	}

	Vector3& operator=(const Vector3& v) {
		x = v.x, y = v.y, z = v.z;
		return *this;
	}

	Vector3& operator=(const DirectX::XMVECTOR& v) {
		DirectX::XMStoreFloat3(&dxVec, v);
		return *this;
	}

	Vector3& operator-=(const Vector3& v){
		x -= v.x, y -= v.y, z -= v.z;
		return *this;
	}

	Vector3& operator*=(float mul){
		x *= mul, y *= mul, z *= mul;
		return *this;
	}

	Vector3& operator/=(float dev){
		x /= dev, y /= dev, z /= dev;
		return *this;
	}

	float lengthSq()  const {
		return x * x + y * y + z * z;
	}

	float length()  const {
		return sqrtf(lengthSq());
	}

	Vector3 normalize()  const {
		return *this / length();
	}
};

}

