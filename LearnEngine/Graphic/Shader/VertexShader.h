#pragma once

namespace LearnEngine {

class VertexShader {
public:
	VertexShader(const char* filePath, const D3D11_INPUT_ELEMENT_DESC* inputElements, UINT numElement);

	void Apply();

private:
	CComPtr<ID3D11InputLayout> inputLayout;
	CComPtr<ID3D11VertexShader> vertexShader = nullptr;
};

}

