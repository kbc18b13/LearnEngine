#include "stdafx.h"
#include "NonSkinModel.h"

namespace LearnEngine {

NonSkinModel::NonSkinModel(std::unique_ptr<std::unique_ptr<Mesh>[]>&& meshArray,
						   UINT arraySize, const MeshExtents& extents) :
	meshArray(std::move(meshArray)), arraySize(arraySize), extents(extents) {}

}
