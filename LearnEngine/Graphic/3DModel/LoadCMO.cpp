#include "stdafx.h"
#include "LoadCMO.h"
#include "NonSkinModel.h"
#include "Mesh.h"
#include "Material3D.h"
#include "Graphic/Shader/LoadShader.h"
#include "Engine.h"
#include "Graphic/Shader/VertexShader.h"
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

	UINT meshCount = loadT<UINT>(input);//メッシュの数

	std::vector<std::unique_ptr<Mesh>> meshArray;
	meshArray.reserve(meshCount);

	for (int iMesh = 0; iMesh < meshCount; iMesh++) {

		skipName(input);//メッシュの名前は今回使用しないためスキップ。

		UINT materialCount = loadT<UINT>(input);

		//マテリアルが格納された配列
		std::unique_ptr<Material3D* []> materialArray(new Material3D * [materialCount] {});

		for (int iMate = 0; iMate < meshCount; iMate++) {
			std::unique_ptr<wchar_t[]> name = loadName(input);//マテリアル名

			std::unique_ptr<Material3DData> mateData(new Material3DData);//マテリアルデータ
			loadT<Material3DData>(input, mateData.get());

			skipName(input);//ピクセルシェーダ名。使わないのでスキップ。

			for (int iTex = 0; iTex < 8; iTex++) {
				skipName(input);//テクスチャ。8枚。今は使わないのでスキップ。
			}

			//デフォルトのシェーダー
			CComPtr<ID3D11PixelShader> pixelShader = loadPixelShader("");

			D3D11_INPUT_ELEMENT_DESC g_VertexDesc[]{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0,                            0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TANGENT",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "COLOR",    0, DXGI_FORMAT_R8G8B8A8_SNORM,     0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEXTURE",  0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
			};

			VertexShader vShader("", g_VertexDesc, 5);

			materialArray[iMate] = new Material3D(std::move(name), vShader, pixelShader, std::move(mateData));
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
				input.seekg(sizeof(SkinningVertex)* (long long)skCount, std::ios_base::cur);//サイズ分スキップ。
			}
		}

		//メッシュエクステンツ(使わない)
		struct MeshExtents {
			float CenterX, CenterY, CenterZ;
			float Radius;

			float MinX, MinY, MinZ;
			float MaxX, MaxY, MaxZ;
		};
		input.seekg(sizeof(MeshExtents), std::ios_base::cur);//サイズ分スキップ。

		//メッシュを作成
		for (int iSub = 0; iSub < submeshCount; iSub++) {
			CComPtr<ID3D11Buffer>& vb = vbArray[submeshArray[iSub].VertexBufferIndex];
			CComPtr<ID3D11Buffer>& ib = ibArray[submeshArray[iSub].IndexBufferIndex];
			UINT startIndex = submeshArray[iSub].StartIndex;
			UINT primCount = submeshArray[iSub].PrimCount;
			std::unique_ptr<Material3D> material(materialArray[submeshArray[iSub].MaterialIndex]);
			meshArray.push_back(std::make_unique<Mesh>(vb, ib, startIndex, primCount, std::move(material)));
		}

	}

	return std::make_unique<NonSkinModel>(std::move(meshArray));
}

}
