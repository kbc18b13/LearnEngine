#pragma once
namespace LearnEngine {
class VertexShader;

class TestPlane {
public:
	TestPlane();
	~TestPlane();
	void Init();
	void Draw();

private:
	CComPtr<ID3D11Buffer> vertexBuf;
	CComPtr<ID3D11Buffer> indexBuf;
	std::unique_ptr<VertexShader> vShader;
	CComPtr<ID3D11PixelShader> pShader;
};
}

