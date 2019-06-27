#include "stdafx.h"
#include "VertexShader.h"
#include "Graphic/Shader/LoadShader.h"
#include "Engine.h"

namespace LearnEngine {

VertexShader::VertexShader(const char* filePath, const D3D11_INPUT_ELEMENT_DESC* inputElements, UINT numElement)
	:inputLayout(inputLayout){
	std::unique_ptr<char[]> vsBinary;
	size_t vsBinSize;

	vertexShader = loadVertexShader(filePath, vsBinary, vsBinSize);

	D3DDevice()->CreateInputLayout(inputElements, numElement, vsBinary.get(), vsBinSize, &inputLayout);
}

void VertexShader::Apply() {
	D3DContext()->IASetInputLayout(inputLayout);
	D3DContext()->VSSetShader(vertexShader, nullptr, 0);
}

}