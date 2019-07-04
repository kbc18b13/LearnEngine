#pragma once

namespace LearnEngine {

class Material3D;

struct NonSkinVertex {
	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT3 Normal;
	DirectX::XMFLOAT4 Tangent;
	UINT color;
	DirectX::XMFLOAT2 TextureCoordinates;
};

class Mesh {
public:
	Mesh(const CComPtr<ID3D11Buffer>& vertexBuf, const CComPtr<ID3D11Buffer>& indexBuf,
		 UINT startIndex, UINT primCount,
		 std::unique_ptr<Material3D>&& material);
	~Mesh();

	void Draw();

private:
	CComPtr<ID3D11Buffer> vertexBuf;
	CComPtr<ID3D11Buffer> indexBuf;
	UINT startIndex;
	UINT primCount;
	std::unique_ptr<Material3D> material;
};

}

