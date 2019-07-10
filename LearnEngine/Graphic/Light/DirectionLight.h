#pragma once
namespace LearnEngine {
class DirectionLight {
public:
	//ゲッターセッター

	void setVec(const Vector3& v) {
		vec = v;
	}

	Vector3 getVec() const{
		return vec;
	}

	void setColor(const Vector4& c) {
		color = c;
	}

	Vector4 getColor() const{
		return color;
	}

	//ディレクションライトの適用

private:
	Vector3 vec;
	Vector4 color;
};
}