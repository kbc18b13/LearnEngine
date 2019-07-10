#pragma once
namespace LearnEngine {
class Material3D;
class VertexShader;

class TestBox {
public:
	TestBox();
	~TestBox();
	void Init();
	void Draw();

private:
	CComPtr<ID3D11Buffer> vertexBuf;
	CComPtr<ID3D11Buffer> indexBuf;
	UINT indexCount = 0;
	std::unique_ptr<VertexShader> vShader;
	CComPtr<ID3D11PixelShader> pShader;
};
}

