#pragma once
namespace LearnEngine {

class Mesh;

class NonSkinModel {
public:
	NonSkinModel();
	~NonSkinModel();

	NonSkinModel(std::vector<std::unique_ptr<Mesh>>&& meshArray);

	void Draw();
private:
	std::vector<std::unique_ptr<Mesh>> meshArray;
};

}

