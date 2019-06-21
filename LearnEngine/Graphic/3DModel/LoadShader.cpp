#include "stdafx.h"
#include "LoadShader.h"
#include <sys/stat.h>
#include <stdio.h>
#include "Engine.h"

namespace LearnEngine {
CComPtr<ID3D11PixelShader> loadPixelShader(const char* filePath) {
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

	D3DDevice()->CreatePixelShader(binary.get(), fileSize, nullptr, &pixelShader);

	return pixelShader;
}

CComPtr<ID3D11VertexShader> loadVertexShader(const char* filePath,
											 std::unique_ptr<char[]>& binaryDist,
											 size_t& binarySize) {

	FILE* file;
	fopen_s(&file, filePath, "rb");

	if (!file) {
		abort();
	}

	{
		struct stat fileStat;
		stat(filePath, &fileStat);
		binarySize = fileStat.st_size;
	}

	binaryDist.reset(new char[binarySize]);

	fread_s(binaryDist.get(), binarySize, 1, binarySize, file);

	CComPtr<ID3D11VertexShader> vertexShader;

	D3DDevice()->CreateVertexShader(binaryDist.get(), binarySize, nullptr, &vertexShader);

	return vertexShader;
}

}
