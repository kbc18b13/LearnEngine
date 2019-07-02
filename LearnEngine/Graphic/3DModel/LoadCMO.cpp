#pragma once
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
	input.read(reinterpret_cast<char*>(dist), sizeof(T) * (long long)arrayLength);
}

std::unique_ptr<wchar_t[]> loadName(std::fstream& input) {
	UINT size = loadT<UINT>(input);
	std::unique_ptr<wchar_t[]> name(new wchar_t[size]);
	input.read(reinterpret_cast<char*>(name.get()), sizeof(wchar_t) * (long long)size);
	return name;
}

void skipName(std::fstream& input) {
	long long size = loadT<UINT>(input);
	input.ignore(size * sizeof(wchar_t));
}

std::unique_ptr<NonSkinModel> loadNonSkinModel(const char* filePath) {
	std::fstream input(filePath, std::ios_base::in || std::ios_base::binary);

	UINT meshCount = loadT<UINT>(input);//���b�V���̐�

	std::vector<std::unique_ptr<Mesh>> meshArray;
	meshArray.reserve(meshCount);

	for (UINT iMesh = 0; iMesh < meshCount; iMesh++) {

		skipName(input);//���b�V���̖��O�͍���g�p���Ȃ����߃X�L�b�v�B

		UINT materialCount = loadT<UINT>(input);

		//�}�e���A�����i�[���ꂽ�z��
		std::unique_ptr<Material3D* []> materialArray(new Material3D * [materialCount] {});

		for (UINT iMate = 0; iMate < materialCount; iMate++) {
			std::unique_ptr<wchar_t[]> name = loadName(input);//�}�e���A����

			std::unique_ptr<Material3DData> mateData(new Material3DData);//�}�e���A���f�[�^
			loadT<Material3DData>(input, mateData.get());

			skipName(input);//�s�N�Z���V�F�[�_���B�g��Ȃ��̂ŃX�L�b�v�B

			for (int iTex = 0; iTex < 8; iTex++) {
				skipName(input);//�e�N�X�`���B8���B���͎g��Ȃ��̂ŃX�L�b�v�B
			}

			//�f�t�H���g�̃V�F�[�_�[
			CComPtr<ID3D11PixelShader> pixelShader = loadPixelShader("Shader\\DefaultPixel.cso");

			D3D11_INPUT_ELEMENT_DESC g_VertexDesc[]{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0,                            0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TANGENT",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "COLOR",    0, DXGI_FORMAT_R8G8B8A8_SNORM,     0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEXTURE",  0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
			};

			VertexShader vShader("Shader\\DefaultVertex.cso", g_VertexDesc, 5);

			materialArray[iMate] = new Material3D(std::move(name), vShader, pixelShader, std::move(mateData));
		}

		//���ꂪ�X�L���A�j���[�V�������f���Ȃ�0�ȊO������
		BYTE isAnim = loadT<BYTE>(input);

		struct SubMesh {
			UINT MaterialIndex;
			UINT IndexBufferIndex;//�ǂ̃C���f�b�N�X�o�b�t�@���g�p���邩(�ǂ̃C���f�b�N�X���g�p���邩�ł͂Ȃ�)
			UINT VertexBufferIndex;//�ǂ̒��_�o�b�t�@���g�p���邩(�ǂ̒��_���g�p���邩�ł͂Ȃ�)
			UINT StartIndex;
			UINT PrimCount;
		};
		//�T�u���b�V���̔z��B����A�T�u���b�V�������ׂă��b�V���Ƃ��Ĉ����̂ŁA���ƂŎ��������B
		UINT submeshCount = loadT<UINT>(input);
		std::unique_ptr<SubMesh[]> submeshArray(new SubMesh[submeshCount]);

		for (UINT iSub = 0; iSub < submeshCount; iSub++) {
			loadT<SubMesh>(input, &submeshArray[iSub]);
		}

		//�C���f�b�N�X�o�b�t�@
		UINT ibCount = loadT<UINT>(input);
		//�C���f�b�N�X�o�b�t�@�̔z��
		std::unique_ptr<CComPtr<ID3D11Buffer>[]> ibArray(new CComPtr<ID3D11Buffer>[ibCount]);

		for (UINT iIb = 0; iIb < ibCount; iIb++) {

			//�t�@�C������USHORT�z���ǂݍ���
			UINT indexCount = loadT<UINT>(input);
			std::unique_ptr<USHORT[]> indexArray(new USHORT[indexCount]);
			loadT<USHORT>(input, indexArray.get(), indexCount);

			//�o�b�t�@�����������āA
			D3D11_BUFFER_DESC bufDesc{};
			bufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bufDesc.ByteWidth = sizeof(USHORT) * indexCount;
			bufDesc.CPUAccessFlags = 0;
			bufDesc.MiscFlags = 0;
			bufDesc.Usage = D3D11_USAGE_DEFAULT;
			//�T�u���\�[�X�f�[�^�ɂ͔z��̐擪�|�C���^�����邾���B
			D3D11_SUBRESOURCE_DATA subR{};
			subR.pSysMem = indexArray.get();
			//�쐬
			D3DDevice()->CreateBuffer(&bufDesc, &subR, &ibArray[iIb]);
		}

		//���_�o�b�t�@
		UINT vbCount = loadT<UINT>(input);
		//���_�o�b�t�@�̔z��
		std::unique_ptr<CComPtr<ID3D11Buffer>[]> vbArray(new CComPtr<ID3D11Buffer>[vbCount]);
		for (UINT iVb = 0; iVb < vbCount; iVb++) {

			//�t�@�C������Vertex�z���ǂݍ���
			UINT vertexCount = loadT<UINT>(input);
			std::unique_ptr<NonSkinVertex[]> vertexArray(new NonSkinVertex[vertexCount]);
			loadT<NonSkinVertex>(input, vertexArray.get(), vertexCount);

			//�o�b�t�@�����������āA
			D3D11_BUFFER_DESC bufDesc{};
			bufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bufDesc.ByteWidth = sizeof(NonSkinVertex) * vertexCount;
			bufDesc.CPUAccessFlags = 0;
			bufDesc.MiscFlags = 0;
			bufDesc.Usage = D3D11_USAGE_DEFAULT;
			//�T�u���\�[�X�f�[�^�ɂ͔z��̐擪�|�C���^�����邾���B
			D3D11_SUBRESOURCE_DATA subR{};
			subR.pSysMem = vertexArray.get();
			//�쐬
			D3DDevice()->CreateBuffer(&bufDesc, &subR, &vbArray[iVb]);
		}

		//�X�L�����_�B�����ł̓X�L�b�v���邾���B
		{
			struct SkinningVertex {
				UINT boneIndex[4];
				float boneWeight[4];
			};
			UINT skbCount = loadT<UINT>(input);
			for (UINT iSkb = 0; iSkb < skbCount; iSkb++) {
				UINT skCount = loadT<UINT>(input);
				input.ignore(sizeof(SkinningVertex)* (long long)skCount);//�T�C�Y���X�L�b�v�B
			}
		}

		//���b�V���G�N�X�e���c(�g��Ȃ�)
		struct MeshExtents {
			float CenterX, CenterY, CenterZ;
			float Radius;

			float MinX, MinY, MinZ;
			float MaxX, MaxY, MaxZ;
		};
		input.ignore(sizeof(MeshExtents));//�T�C�Y���X�L�b�v�B

		//���b�V�����쐬
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
