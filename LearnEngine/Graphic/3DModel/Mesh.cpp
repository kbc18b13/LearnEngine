#include "stdafx.h"
#include "Mesh.h"
#include "Material3D.h"
#include "Graphic/GraphicEngine.h"

namespace LearnEngine {

Mesh::Mesh(const CComPtr<ID3D11Buffer>& vertexBuf, const CComPtr<ID3D11Buffer>& indexBuf,
		   UINT startIndex, UINT primCount,
		   std::unique_ptr<Material3D>&& material) :
	vertexBuf(vertexBuf), indexBuf(indexBuf), material(std::move(material)),
	startIndex(startIndex), primCount(primCount) {
}

Mesh::~Mesh() {
}

void Mesh::Draw() {
	material->Apply();

	UINT vertexSize = sizeof(NonSkinVertex);
	UINT offset = 0;
	D3DContext()->IASetIndexBuffer(indexBuf, DXGI_FORMAT_R16_UINT, 0);
	D3DContext()->IASetVertexBuffers(0, 1, &vertexBuf.p, &vertexSize, &offset);
	D3DContext()->DrawIndexed(primCount * 3, startIndex, 0);
}

}