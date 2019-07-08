#include "Material3D.h"
#include "Engine.h"
#include "Graphic/Shader/VertexShader.h"
#include "Graphic/Texture/Texture.h"

namespace LearnEngine {
Material3D::~Material3D() {
}

Material3D::Material3D(std::unique_ptr<wchar_t[]>&& name,
					   const VertexShader& vs,
					   const CComPtr<ID3D11PixelShader>& pShader,
					   std::unique_ptr<Material3DData>&& data,
					   std::unique_ptr<Texture> (&&texArray8)[8],
					   const D3D11_SAMPLER_DESC& samplerDesc)
	: name(std::move(name)), vShader(vs), pShader(pShader), data(std::move(data)){
	for (int i = 0; i < 8; i++) {
		textures[i] = std::move(texArray8[i]);
	}
	HRESULT hr = D3DDevice()->CreateSamplerState(&samplerDesc, &sampler);
	if (FAILED(hr)) {
		abort();
	}
}

void Material3D::Apply() {
	vShader.Apply();
	D3DContext()->PSSetShader(pShader, nullptr, 0);

	for (int i = 0; i < 8; i++) {
		textures[i]->Apply(i);
	}
}

}
