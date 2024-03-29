#include "stdafx.h"
#include "LoadShader.h"
#include <sys/stat.h>
#include <stdio.h>
#include "Graphic/GraphicEngine.h"

namespace LearnEngine {
CComPtr<ID3D11PixelShader> loadPixelShader(const char* filePath) {
	HRESULT res;
	FILE* file;
	fopen_s(&file, filePath, "rb");

	if (!file) {
		abort();
	}

	size_t fileSize;
	{
		struct stat fileStat;
		stat(filePath, &fileStat);
		fileSize = fileStat.st_size;
	}

	std::unique_ptr<char[]> binary(new char[fileSize]);

	fread_s(binary.get(), fileSize, 1, fileSize, file);

	CComPtr<ID3D11PixelShader> pixelShader;

	res = D3DDevice()->CreatePixelShader(binary.get(), fileSize, nullptr, &pixelShader);
	if (FAILED(res)) {
		abort();
	}
	return pixelShader;
}

CComPtr<ID3D11VertexShader> loadVertexShader(const char* filePath,
											 std::unique_ptr<char[]>& binaryDist,
											 size_t& binarySizeDist) {
	HRESULT res;
	FILE* file;
	fopen_s(&file, filePath, "rb");

	if (!file) {
		abort();
	}

	{
		struct stat fileStat;
		stat(filePath, &fileStat);
		binarySizeDist = fileStat.st_size;
	}

	binaryDist.reset(new char[binarySizeDist]);

	fread_s(binaryDist.get(), binarySizeDist, 1, binarySizeDist, file);

	CComPtr<ID3D11VertexShader> vertexShader;

	res = D3DDevice()->CreateVertexShader(binaryDist.get(), binarySizeDist, nullptr, &vertexShader);
	if (FAILED(res)) {
		abort();
	}
	return vertexShader;
}

}
