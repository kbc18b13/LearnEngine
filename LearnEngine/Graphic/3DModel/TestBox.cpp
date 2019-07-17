#include "stdafx.h"
#include "TestBox.h"
#include "Graphic/GraphicEngine.h"
#include "Graphic/Shader/LoadShader.h"
#include "Graphic/Shader/VertexShader.h"

namespace LearnEngine {

struct NonSkinVertex {
	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT3 Normal;
	DirectX::XMFLOAT4 Tangent;
	DirectX::XMFLOAT4 color;
	DirectX::XMFLOAT2 TextureCoordinates;
};

TestBox::TestBox() {
}

TestBox::~TestBox() {
}

void TestBox::Init() {
	HRESULT res;
	NonSkinVertex vArray[24]{};
	float v[3];
	for (int i = 0; i < 3; i++) {
		int x;
		if (3 == (x = i + 1))x = 0;
		int y;
		if (-1 == (y = i - 1))y = 2;

		for (int j = 0; j < 2; j++) {
			v[i] = (1 == j) ? -25.f : 25.f;

			for (int k = 0; k < 4; k++) {
				v[x] = (2 <= k) ? -25.f : 25.f;
				v[y] = (1 == k || 2 == k) ? -25.f : 25.f;
				vArray[i * 8 + j * 4 + k].Position = { v[0], v[1], v[2] };

				vArray[i * 8 + j * 4 + k].color.w = 1;
				if (i == 0) {
					vArray[i * 8 + j * 4 + k].color.x = 1;
				}else if (i == 1) {
					vArray[i * 8 + j * 4 + k].color.y = 1;
				}else if (i == 2) {
					vArray[i * 8 + j * 4 + k].color.z = 1;
				}
			}
		}
	}

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

	USHORT indArray[36] = {
		 0, 1, 2,   3, 0, 2,   4, 5, 6,   7,  4, 6,   8, 9,10,  11, 8,10,
		12,13,14,  15,12,14,  16,17,18,  19, 16,18,  20,21,22,  23,20,22
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
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXTURE",  0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

 	vShader.reset(new VertexShader("Shader\\DefaultVertex.cso", g_VertexDesc, 5));
}

void TestBox::Draw() {
	vShader->Apply();
	D3DContext()->PSSetShader(pShader, nullptr, 0);
	D3DContext()->IASetIndexBuffer(indexBuf, DXGI_FORMAT_R16_UINT, 0);
	UINT size = sizeof(NonSkinVertex);
	UINT zero = 0;
	D3DContext()->IASetVertexBuffers(0, 1, &vertexBuf.p, &size, &zero);

	D3DContext()->DrawIndexed(36, 0, 0);
}

}