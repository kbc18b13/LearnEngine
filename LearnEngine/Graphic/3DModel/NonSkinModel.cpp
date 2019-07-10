#include "stdafx.h"
#include "NonSkinModel.h"
#include "Mesh.h"
#include "Graphic/ConstBuffer/ConstBuffer.h"
#include "Engine.h"


namespace LearnEngine {
NonSkinModel::NonSkinModel() {
}
NonSkinModel::~NonSkinModel() {
}

NonSkinModel::NonSkinModel(std::vector<std::unique_ptr<Mesh>>&& meshArray) :
	meshArray(std::move(meshArray)) {}

void NonSkinModel::UpdateWorldMatrix(const Matrix& mat) {
	ConstBuffer& cBuffer = Engine().getGraphic().getCBuffer();
	cBuffer.setWorldMatrix(mat);
	cBuffer.UpdateObject();
}

void NonSkinModel::Draw() {
	for (std::unique_ptr<Mesh>& mesh : meshArray) {
		mesh->Draw();
	}
}

}
