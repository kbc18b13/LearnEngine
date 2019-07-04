#include "stdafx.h"
#include "TestPlane.h"
#include "Engine.h"
#include "Graphic/Shader/LoadShader.h"
#include "Graphic/Shader/VertexShader.h"

namespace LearnEngine {

struct NonSkinVertex {
	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT3 Normal;
	DirectX::XMFLOAT4 Tangent;
	UINT color;
	DirectX::XMFLOAT2 TextureCoordinates;
};

TestPlane::TestPlane() {
}

TestPlane::~TestPlane() {
}

void TestPlane::Init() {
	HRESULT res;
	NonSkinVertex vArray[4]{};
	vArray[0].Position = {  10, 10, 0 };
	vArray[1].Position = {  10,-10, 0 };
	vArray[2].Position = { -10,-10, 0 };
	vArray[3].Position = { -10, 10, 0 };

	D3D11_BUFFER_DESC bufDesc{};
	bufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufDesc.ByteWidth = ARRAYSIZE(vArray) * sizeof(NonSkinVertex);
	bufDesc.CPUAccessFlags = 0;
	bufDesc.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA data{};
	data.pSysMem = vArray;

	res = D3DDevice()->CreateBuffer(&bufDesc, &data, &vertexBuf);
	if (FAILED(res)) {
		abort();
	}
	
	USHORT indArray[6] = {
		 0, 1, 2,   3, 0, 2
	};

	bufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufDesc.ByteWidth = ARRAYSIZE(indArray) * sizeof(USHORT);

	data.pSysMem = indArray;

	res = D3DDevice()->CreateBuffer(&bufDesc, &data, &indexBuf);
	if (FAILED(res)) {
		abort();
	}

	pShader = loadPixelShader("Shader\\DefaultPixel.cso");

	D3D11_INPUT_ELEMENT_DESC g_VertexDesc[]{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0,                            0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_R8G8B8A8_SNORM,     0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXTURE",  0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	vShader.reset(new VertexShader("Shader\\DefaultVertex.cso", g_VertexDesc, 5));
}

void TestPlane::Draw() {
	vShader->Apply();
	D3DContext()->PSSetShader(pShader, nullptr, 0);
	D3DContext()->IASetIndexBuffer(indexBuf, DXGI_FORMAT_R16_UINT, 0);
	UINT size = sizeof(NonSkinVertex);
	UINT zero = 0;
	D3DContext()->IASetVertexBuffers(0, 1, &vertexBuf.p, &size, &zero);

	D3DContext()->DrawIndexed(6, 0, 0);
}

}