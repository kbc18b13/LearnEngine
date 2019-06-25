#pragma once
namespace LearnEngine {

class Mesh;

class NonSkinModel {
public:
	NonSkinModel(std::vector<std::unique_ptr<Mesh>>&& meshArray);

private:
	std::vector<std::unique_ptr<Mesh>> meshArray;
};

}

