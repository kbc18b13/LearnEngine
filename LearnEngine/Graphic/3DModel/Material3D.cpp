#include "Material3D.h"
#include "Engine.h"
#include "Graphic/Shader/VertexShader.h"

namespace LearnEngine {
Material3D::Material3D(std::unique_ptr<wchar_t[]>&& name,
					   const VertexShader& vs,
					   const CComPtr<ID3D11PixelShader>& pShader,
					   std::unique_ptr<Material3DData>&& data)
	: name(std::move(name)), vShader(vs), pShader(pShader), data(std::move(data)){
}

void Material3D::Apply() {
	vShader.Apply();
	D3DContext()->PSSetShader(pShader, nullptr, 0);
}

}
