#pragma once
#include "stdafx.h"
#include "Engine.h"
#include <fstream>
#include <string>

#include "LoadCMO.h"
#include "NonSkinModel.h"
#include "Mesh.h"
#include "Material3D.h"
#include "Graphic/Shader/LoadShader.h"
#include "Graphic/Shader/VertexShader.h"

#include "Graphic/Texture/Texture.h"

namespace LearnEngine {

template<typename T>
T loadT(std::fstream& input) {
	T value;
	input.read(reinterpret_cast<char*>(&value), sizeof(T));
	return value;
}

template<typename T>
inline void loadT(std::fstream& input, T* dist, UINT arrayLength = 1) {
	input.read(reinterpret_cast<char*>(dist), sizeof(T) * (long long)arrayLength);
}

std::unique_ptr<wchar_t[]> loadName(std::fstream& input) {
	UINT size = loadT<UINT>(input);
	std::unique_ptr<wchar_t[]> name(new wchar_t[size]);
	input.read(reinterpret_cast<char*>(name.get()), sizeof(wchar_t) * (long long)size);
	return name;
}

std::unique_ptr<wchar_t[]> combineLoadName(const wchar_t* front ,std::fstream& input) {
	const UINT size = loadT<UINT>(input);
	const size_t frontLength = wcslen(front);
	std::unique_ptr<wchar_t[]> name(new wchar_t[size + frontLength]);
	wchar_t* loadP = name.get();
	wcscpy_s(name.get(), frontLength+1, front);
	loadP += frontLength;
	input.read(reinterpret_cast<char*>(loadP), sizeof(wchar_t) * (long long)size);
	return name;
}

void skipName(std::fstream& input) {
	long long size = loadT<UINT>(input);
	input.ignore(size * sizeof(wchar_t));
}

std::unique_ptr<NonSkinModel> loadNonSkinModel(const char* filePath) {
	std::fstream input(filePath, std::ios_base::in | std::ios_base::binary);

	UINT meshCount = loadT<UINT>(input);//メッシュの数

	std::vector<std::unique_ptr<Mesh>> meshArray;
	meshArray.reserve(meshCount);

	for (UINT iMesh = 0; iMesh < meshCount; iMesh++) {

		skipName(input);//メッシュの名前は今回使用しないためスキップ。

		UINT materialCount = loadT<UINT>(input);

		//マテリアルが格納された配列
		std::unique_ptr<Material3D* []> materialArray(new Material3D * [materialCount] {});

		for (UINT iMate = 0; iMate < materialCount; iMate++) {
			std::unique_ptr<wchar_t[]> name = loadName(input);//マテリアル名

			std::unique_ptr<Material3DData> mateData(new Material3DData);//マテリアルデータ
			loadT<Material3DData>(input, mateData.get());

			skipName(input);//ピクセルシェーダ名。使わないのでスキップ。

			//テクスチャーを8つ読み込む
			std::unique_ptr<Texture> textures[8];
			for (int iTex = 0; iTex < 8; iTex++) {
				textures[iTex] = std::make_unique<Texture>();
				textures[iTex]->loadTexture(combineLoadName(L"ModelData\\", input).get());
			}

			//デフォルトのサンプラー
			D3D11_SAMPLER_DESC smpDesc{};
			smpDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			smpDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
			smpDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
			smpDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

			//デフォルトのシェーダー
			CComPtr<ID3D11PixelShader> pixelShader = loadPixelShader("Shader\\DefaultPixel.cso");

			D3D11_INPUT_ELEMENT_DESC g_VertexDesc[]{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0,                            0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TANGENT",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "COLOR",    0, DXGI_FORMAT_R8G8B8A8_SNORM,     0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEXTURE",  0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
			};

			VertexShader vShader("Shader\\DefaultVertex.cso", g_VertexDesc, 5);

			materialArray[iMate] = new Material3D(std::move(name), vShader, pixelShader, std::move(mateData),
												  std::move(textures), smpDesc);
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

		for (UINT iSub = 0; iSub < submeshCount; iSub++) {
			loadT<SubMesh>(input, &submeshArray[iSub]);
		}

		//インデックスバッファ
		UINT ibCount = loadT<UINT>(input);
		//インデックスバッファの配列
		std::unique_ptr<CComPtr<ID3D11Buffer>[]> ibArray(new CComPtr<ID3D11Buffer>[ibCount]);

		for (UINT iIb = 0; iIb < ibCount; iIb++) {

			//ファイルからUSHORT配列を読み込む
			UINT indexCount = loadT<UINT>(input);
			std::unique_ptr<USHORT[]> indexArray(new USHORT[indexCount]);
			for (UINT i = 0; i < indexCount; i++) {
				indexArray[i] = 0;
			}
			loadT<USHORT>(input, indexArray.get(), indexCount);
			int nnnn = input.rdstate();
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
		std::unique_ptr<CComPtr<ID3D11Buffer>[]> vbArray(new CComPtr<ID3D11Buffer>[vbCount]);
		for (UINT iVb = 0; iVb < vbCount; iVb++) {

			//ファイルからVertex配列を読み込む
			UINT vertexCount = loadT<UINT>(input);
			std::unique_ptr<NonSkinVertex[]> vertexArray(new NonSkinVertex[vertexCount]);
			loadT<NonSkinVertex>(input, vertexArray.get(), vertexCount);

			//逆さまになってるテクスチャ座標を修正(なんで逆さまなのかしらん)
			for (UINT v = 0; v < vertexCount; v++) {
				vertexArray[v].TextureCoordinates.y = 1.0f - vertexArray[v].TextureCoordinates.y;
			}

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
			for (UINT iSkb = 0; iSkb < skbCount; iSkb++) {
				UINT skCount = loadT<UINT>(input);
				input.ignore(sizeof(SkinningVertex)* (long long)skCount);//サイズ分スキップ。
			}
		}

		//メッシュエクステンツ(使わない)
		struct MeshExtents {
			float CenterX, CenterY, CenterZ;
			float Radius;

			float MinX, MinY, MinZ;
			float MaxX, MaxY, MaxZ;
		};
		input.ignore(sizeof(MeshExtents));//サイズ分スキップ。

		//ボーンとアニメーションをスキップ。ノンスキンモデルでは用は無いので。
		if (isAnim) {
			struct Bone {
				UINT ParentIndex;
				DirectX::XMFLOAT4X4 InvBindPos;
				DirectX::XMFLOAT4X4 BindPos;
				DirectX::XMFLOAT4X4 LocalTransform;
			};
			UINT boneCount = loadT<UINT>(input);
			for (UINT i = 0; i < boneCount; i++) {
				skipName(input);
				input.ignore(sizeof(Bone));//ボーンをスキップ
			}

			struct Keyframe {
				UINT BoneIndex;
				float Time;
				DirectX::XMFLOAT4X4 Transform;
			};
			UINT clipCount = loadT<UINT>(input);
			for (UINT i = 0; i < clipCount; i++) {
				skipName(input);
				input.ignore(sizeof(float)*2);//スタートとエンドのタイム情報をスキップ

				UINT keyCount = loadT<UINT>(input);
				for (UINT k = 0; k < keyCount; k++) {
					input.ignore(sizeof(Keyframe));//キーフレームをスキップ
				}
			}
		}
		//ここまでボーンとアニメーションのスキップ。


		//メッシュを作成
		for (UINT iSub = 0; iSub < submeshCount; iSub++) {
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
