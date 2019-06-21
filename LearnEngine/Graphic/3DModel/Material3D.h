namespace LearnEngine {

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
	Material3D();
	void Init(std::unique_ptr<wchar_t[]>&& name,
		const CComPtr<ID3D11PixelShader>& pShader,
			  std::unique_ptr<Material3DData>&& data);

private:
	std::unique_ptr<wchar_t[]> name;
	CComPtr<ID3D11PixelShader> pShader;
	std::unique_ptr<Material3DData> data;
};
}