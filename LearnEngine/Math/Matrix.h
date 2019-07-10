#pragma once
namespace LearnEngine {

struct Matrix {
	DirectX::XMFLOAT4X4 dxMat;

	Matrix() : dxMat(0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0){ }

	Matrix(DirectX::XMMATRIX mat) {
		DirectX::XMStoreFloat4x4(&dxMat, mat);
	}

	Matrix(const Matrix& mat) : dxMat(mat.dxMat){}

	operator DirectX::XMMATRIX() {
		return DirectX::XMLoadFloat4x4(&dxMat);
	}

	Matrix& operator=(const Matrix& mat) {
		dxMat = mat.dxMat;
		return *this;
	}

	Matrix& operator=(const DirectX::XMMATRIX mat) {
		DirectX::XMStoreFloat4x4(&dxMat, mat);
		return *this;
	}

	Matrix operator*(const DirectX::XMMATRIX mat) {
		return DirectX::XMLoadFloat4x4(&dxMat) * mat;
	}

//////////////staticä÷êî/////////////////////

	static Matrix getWorldMatrix(const Vector3& pos, const Quaternion& rot, const Vector3& scale) {
		DirectX::XMMATRIX mat = DirectX::XMMatrixScalingFromVector(scale);
		mat = DirectX::XMMatrixMultiply(mat, DirectX::XMMatrixRotationQuaternion(rot));
		mat = DirectX::XMMatrixMultiply(mat, DirectX::XMMatrixTranslationFromVector(pos));
		return mat;
	}
};

}