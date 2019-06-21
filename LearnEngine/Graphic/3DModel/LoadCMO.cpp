#include "stdafx.h"
#include "LoadCMO.h"
#include "NonSkinModel.h"
#include "Mesh.h"
#include "Material3D.h"
#include "LoadShader.h"
#include "Engine.h"
#include <fstream>

namespace LearnEngine {

template<typename T>
T loadT(std::fstream& input) {
	T value;
	input.read(reinterpret_cast<char*>(&value), sizeof(T));
	return value;
}

template<typename T>
void loadT(std::fstream& input, T* dist, UINT arrayLength = 1) {
	input.read(reinterpret_cast<char*>(&dist), sizeof(T) * (long long)arrayLength);
}

std::unique_ptr<wchar_t[]> loadName(std::fstream& input) {
	long long size = loadT<UINT>(input);
	std::unique_ptr<wchar_t[]> name(new wchar_t[size]);
	input.read(reinterpret_cast<char*>(name.get()), sizeof(wchar_t) * size);
	return name;
}

void skipName(std::fstream& input) {
	long long size = loadT<UINT>(input);
	input.read(reinterpret_cast<char*>(&size), sizeof(UINT));
	input.seekg(sizeof(wchar_t) * size, std::ios_base::cur);
}

std::unique_ptr<NonSkinModel> loadNonSkinModel(const char* filePath) {
	std::fstream input(filePath, std::ios_base::in || std::ios_base::binary);

	UINT meshCount = loadT<UINT>(input);

	for (int iMesh = 0; iMesh < meshCount; iMesh++) {

		skipName(input);//メッシュの名前は今回使用しないためスキップ。

		UINT materialCount = loadT<UINT>(input);

		//マテリアルが格納された配列
		std::unique_ptr<Material3D[]> materialArray(new Material3D[materialCount]);

		for (int iMate = 0; iMate < meshCount; iMate++) {
			std::unique_ptr<wchar_t[]> name = loadName(input);//名前

			std::unique_ptr<Material3DData> mateData(new Material3DData);//マテリアルデータ
			loadT<Material3DData>(input, mateData.get());

			skipName(input);//ピクセルシェーダ名。使わないのでスキップ。

			for (int iTex = 0; iTex < 8; iTex++) {
				skipName(input);//テクスチャ。8枚。今は使わないのでスキップ。
			}

			CComPtr<ID3D11PixelShader> pixelShader = loadPixelShader("");

			materialArray[iMate].Init(std::move(name), pixelShader, std::move(mateData));
		}

		//これがスキンアニメーションモデルなら0以外が入る
		BYTE isAnim = loadT<BYTE>(input);

		struct SubMesh {
			UINT MaterialIndex;
			UINT IndexBufferIndex;//どのインデックスバッファを使用するか(どのインデックスを使用するかではない)
			UINT VertexBufferIndex;//どの頂点バッファを使用するか(どの頂点を使用するかではない)
			UINT StartIndex;
			UINT PrimCount;
		};
		//サブメッシュの配列。今回、サブメッシュをすべてメッシュとして扱うので、あとで手を加える。
		UINT submeshCount = loadT<UINT>(input);
		std::unique_ptr<SubMesh[]> submeshArray(new SubMesh[submeshCount]);

		for (int iSub = 0; iSub < submeshCount; iSub++) {
			loadT<SubMesh>(input, &submeshArray[iSub]);
		}

		//インデックスバッファ
		UINT ibCount = loadT<UINT>(input);
		//インデックスバッファの配列
		std::unique_ptr<CComPtr<ID3D11Buffer>[]> ibArray(new CComPtr<ID3D11Buffer>[ibCount]);

		for (int iIb = 0; iIb < ibCount; iIb++) {

			//ファイルからUSHORT配列を読み込む
			UINT indexCount = loadT<UINT>(input);
			std::unique_ptr<USHORT[]> indexArray(new USHORT[indexCount]);
			loadT<USHORT>(input, indexArray.get(), indexCount);

			//バッファ説明を書いて、
			D3D11_BUFFER_DESC bufDesc{};
			bufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bufDesc.ByteWidth = sizeof(USHORT) * indexCount;
			bufDesc.CPUAccessFlags = 0;
			bufDesc.MiscFlags = 0;
			bufDesc.Usage = D3D11_USAGE_DEFAULT;
			//サブリソースデータには配列の先頭ポインタを入れるだけ。
			D3D11_SUBRESOURCE_DATA subR{};
			subR.pSysMem = indexArray.get();
			//作成
			D3DDevice()->CreateBuffer(&bufDesc, &subR, &ibArray[iIb]);
		}

		//頂点バッファ
		UINT vbCount = loadT<UINT>(input);
		//頂点バッファの配列
		std::unique_ptr<CComPtr<ID3D11Buffer>[]> vbArray(new CComPtr<ID3D11Buffer>[ibCount]);
		for (int iVb = 0; iVb < vbCount; iVb++) {

			//ファイルからVertex配列を読み込む
			UINT vertexCount = loadT<UINT>(input);
			std::unique_ptr<NonSkinVertex[]> vertexArray(new NonSkinVertex[vertexCount]);
			loadT<NonSkinVertex>(input, vertexArray.get(), vertexCount);

			//バッファ説明を書いて、
			D3D11_BUFFER_DESC bufDesc{};
			bufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bufDesc.ByteWidth = sizeof(NonSkinVertex) * vertexCount;
			bufDesc.CPUAccessFlags = 0;
			bufDesc.MiscFlags = 0;
			bufDesc.Usage = D3D11_USAGE_DEFAULT;
			//サブリソースデータには配列の先頭ポインタを入れるだけ。
			D3D11_SUBRESOURCE_DATA subR{};
			subR.pSysMem = vertexArray.get();
			//作成
			D3DDevice()->CreateBuffer(&bufDesc, &subR, &vbArray[iVb]);
		}

		//スキン頂点。ここではスキップするだけ。
		{
			struct SkinningVertex {
				UINT boneIndex[4];
				float boneWeight[4];
			};
			UINT skbCount = loadT<UINT>(input);
			for (int iSkb = 0; iSkb < skbCount; iSkb++) {
				UINT skCount = loadT<UINT>(input);
				input.seekg(sizeof(SkinningVertex)* skCount, std::ios_base::cur);//サイズ分スキップ。
			}
		}

		//メッシュエクステンツ
		MeshExtents extents;
		loadT<MeshExtents>(input, &extents);

	}
}

}
