#pragma once
#include "Graphic/Shader/VertexShader.h"

namespace LearnEngine {

class Texture;

struct Material3DData {
	DirectX::XMFLOAT4   Ambient;
	DirectX::XMFLOAT4   Diffuse;
	DirectX::XMFLOAT4   Specular;
	float               SpecularPower;
	DirectX::XMFLOAT4   Emissive;
	DirectX::XMFLOAT4X4 UVTransform;
};

class Material3D {
public:
	Material3D(std::unique_ptr<wchar_t[]>&& name,
			   const VertexShader& vs,
			   const CComPtr<ID3D11PixelShader>& pShader,
			   std::unique_ptr<Material3DData>&& data,
			   std::unique_ptr<Texture> (&&texArray) [8],
			   const D3D11_SAMPLER_DESC& samplerDesc);

	~Material3D();

	void Apply();

private:
	std::unique_ptr<wchar_t[]> name;
	VertexShader vShader;
	CComPtr<ID3D11PixelShader> pShader;
	std::unique_ptr<Material3DData> data;
	std::unique_ptr<Texture> textures[8];
	CComPtr<ID3D11SamplerState> sampler;
};
}