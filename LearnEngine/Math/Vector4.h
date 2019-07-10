#pragma once
namespace LearnEngine {

union Vector4 {
	DirectX::XMFLOAT4 dxVec;
	struct {
		float x;
		float y;
		float z;
		float w;
	};

	Vector4() : x(0), y(0), z(0), w(0) {}

	Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

	Vector4(const Vector4& v) : dxVec(v.dxVec) {}

	Vector4(const DirectX::XMVECTOR& v) {
		DirectX::XMStoreFloat4(&dxVec, v);
	}

	operator DirectX::XMVECTOR() const {
		return DirectX::XMLoadFloat4(&dxVec);
	}

	Vector4 operator+(const Vector4& v) const {
		return Vector4(x + v.x, y + v.y, z + v.z, w + v.w);
	}

	Vector4 operator-(const Vector4& v)  const {
		return Vector4(x - v.x, y - v.y, z - v.z, w - v.w);
	}

	Vector4 operator*(float mul)  const {
		return Vector4(x * mul, y * mul, z * mul, w * mul);
	}

	Vector4 operator/(float dev)  const {
		return Vector4(x / dev, y / dev, z / dev, w / dev);
	}

	Vector4& operator=(const Vector4& v) {
		x = v.x, y = v.y, z = v.z, w = v.w;
		return *this;
	}

	Vector4& operator=(const DirectX::XMVECTOR& v) {
		DirectX::XMStoreFloat4(&dxVec, v);
		return *this;
	}

	Vector4& operator+=(const Vector4& v) {
		x += v.x, y += v.y, z += v.z, w += v.w;
		return *this;
	}

	Vector4& operator-=(const Vector4& v) {
		x -= v.x, y -= v.y, z -= v.z, w += v.w;
		return *this;
	}

	Vector4& operator*=(float mul) {
		x *= mul, y *= mul, z *= mul, w *= mul;
		return *this;
	}

	Vector4& operator/=(float dev) {
		x /= dev, y /= dev, z /= dev, w /= dev;
		return *this;
	}

	float lengthSq()  const {
		return x * x + y * y + z * z + w * w;
	}

	float length()  const {
		return sqrtf(lengthSq());
	}

	Vector4 normalize()  const {
		return *this / length();
	}

	///////////////////////Ç±Ç±Ç©ÇÁstaticä÷êî//////////////////////////////

	static Vector4 Red() {
		return Vector4(1, 0, 0, 1);
	}

	static Vector4 Green() {
		return Vector4(0, 1, 0, 1);
	}

	static Vector4 Blue() {
		return Vector4(0, 0, 1, 1);
	}

	static Vector4 White() {
		return Vector4(1, 1, 1, 1);
	}

	static Vector4 Black() {
		return Vector4(0, 0, 0, 1);
	}
};

}