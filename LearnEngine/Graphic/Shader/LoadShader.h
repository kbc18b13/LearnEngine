#pragma once
namespace LearnEngine {

CComPtr<ID3D11PixelShader> loadPixelShader(const char* filePath);

CComPtr<ID3D11VertexShader> loadVertexShader(const char* filePath,
											 std::unique_ptr<char[]>& binaryDist,
											 size_t& binarySizeDist);

}
