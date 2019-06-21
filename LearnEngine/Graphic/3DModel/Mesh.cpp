#include "stdafx.h"
#include "Mesh.h"

namespace LearnEngine {

Mesh::Mesh(const CComPtr<ID3D11Buffer>& vertexBuf, const CComPtr<ID3D11Buffer>& indexBuf,
		   UINT startIndex, UINT primCount,
		   std::unique_ptr<Material3DData>&& material) :
	vertexBuf(vertexBuf), indexBuf(indexBuf), material(std::move(material)),
	startIndex(startIndex), primCount(primCount) {
}

Mesh::~Mesh() {
}

}