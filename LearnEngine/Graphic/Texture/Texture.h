#pragma once
namespace LearnEngine {
class Texture {
public:
	Texture();
	~Texture();
	void loadTexture(const wchar_t* filePath);
	void Apply(UINT slot);

private:
	CComPtr<ID3D11Resource> texRes;
	CComPtr<ID3D11ShaderResourceView> texRV;
};
}
