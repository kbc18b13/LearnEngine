#include "stdafx.h"
#include "Texture.h"
#include "DDSTextureLoader.h"
#include "Graphic/GraphicEngine.h"

namespace LearnEngine {

Texture::Texture() {
}

Texture::~Texture() {
}

void Texture::loadTexture(const wchar_t* filePath) {
	using namespace DirectX;
	HRESULT res = CreateDDSTextureFromFile(D3DDevice(), filePath, &texRes, &texRV);
	if (FAILED(res)) {
		//TODO ‚Ç‚¤‚µ‚æ
	}
}

void Texture::Apply(UINT slot) {
	if (texRV.p != nullptr) {
		D3DContext()->PSSetShaderResources(slot, 1, &texRV.p);
	}
}

}
