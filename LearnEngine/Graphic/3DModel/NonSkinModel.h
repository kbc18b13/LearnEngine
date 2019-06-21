#pragma once
namespace LearnEngine {

class Mesh;

struct MeshExtents {
	float CenterX, CenterY, CenterZ;
	float Radius;

	float MinX, MinY, MinZ;
	float MaxX, MaxY, MaxZ;
};

class NonSkinModel {
public:
	NonSkinModel(std::unique_ptr<std::unique_ptr<Mesh>[]>&& meshArray,
				 UINT arraySize, const MeshExtents& extents);

private:
	std::unique_ptr<std::unique_ptr<Mesh>[]> meshArray;
	UINT arraySize;

	MeshExtents extents;  //�o�E���f�B���O�{�b�N�X, �o�E���f�B���O�X�t�B�A�̏��
};

}

