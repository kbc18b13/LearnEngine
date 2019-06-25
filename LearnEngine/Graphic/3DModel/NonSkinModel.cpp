#include "stdafx.h"
#include "NonSkinModel.h"

namespace LearnEngine {

NonSkinModel::NonSkinModel(std::vector<std::unique_ptr<Mesh>>&& meshArray) :
	meshArray(std::move(meshArray)) {}

}
