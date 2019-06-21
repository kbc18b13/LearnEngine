#include "Material3D.h"
namespace LearnEngine {
Material3D::Material3D() {
}
void Material3D::Init(std::unique_ptr<wchar_t[]>&& name_,
					   const CComPtr<ID3D11PixelShader>& pShader_,
					   std::unique_ptr<Material3DData>&& data_){
	name = std::move(name_);
	pShader = pShader_;
	data = std::move(data_);
}

}
