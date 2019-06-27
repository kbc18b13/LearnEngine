#include "stdafx.h"
#include "NonSkinModel.h"
#include "Mesh.h"

namespace LearnEngine {
NonSkinModel::NonSkinModel() {
}
NonSkinModel::~NonSkinModel() {
}

NonSkinModel::NonSkinModel(std::vector<std::unique_ptr<Mesh>>&& meshArray) :
	meshArray(std::move(meshArray)) {}

void NonSkinModel::Draw() {
	for (std::unique_ptr<Mesh>& mesh : meshArray) {
		mesh->Draw();
	}
}

}
