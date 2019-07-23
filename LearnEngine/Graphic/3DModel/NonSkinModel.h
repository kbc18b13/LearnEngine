#pragma once
namespace LearnEngine {

class Mesh;

class NonSkinModel {
public:
	NonSkinModel();
	~NonSkinModel();

	NonSkinModel(std::vector<std::unique_ptr<Mesh>>&& meshArray);

	void UpdateWorldMatrix(const Matrix& mat);

	void UpdateWorldMatrix(const Vector3& pos,
						   const Quaternion& rot = Quaternion::Identity(),
						   const Vector3& scale = Vector3::Identity()) {
		UpdateWorldMatrix(DirectX::XMMatrixTranspose(Matrix::getWorldMatrix(pos, rot, scale)));
	}

	void Draw();
private:
	std::vector<std::unique_ptr<Mesh>> meshArray;

#ifdef _DEBUG
	bool enableWorldMat = false;
#endif
};

}

