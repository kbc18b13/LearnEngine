#pragma once
#include "Camera/Camera.h"

namespace LearnEngine {
class NonSkinModel;

class GraphicEngine {
public:
	GraphicEngine();
	~GraphicEngine();

	void Init(HWND hWnd);

	void Render();

	ID3D11Device* getDevice() {
		return d3dDevice;
	}

	ID3D11DeviceContext* getContext() {
		return d3dContext;
	}

private:
	CComPtr<IDXGISwapChain>      swapChain;    //�X���b�v�`�F�C��
	CComPtr<ID3D11Device>        d3dDevice;    //d3d�f�o�C�X
	CComPtr<ID3D11DeviceContext> d3dContext;   //d3d�f�o�C�X�R���e�L�X�g
	D3D_FEATURE_LEVEL level;                   //DirectX�̑Ή����x��

	CComPtr<ID3D11Texture2D> depthStencil;
	CComPtr<ID3D11DepthStencilView> dsView;

	CComPtr <ID3D11RenderTargetView> targetView;//�����_�[�^�[�Q�b�g�r���[

	CComPtr<ID3D11Buffer> projCBuf;          //�v���W�F�N�V�����s��萔�o�b�t�@
	CComPtr<ID3D11Buffer> viewCBuf;          //�r���[�s��萔�o�b�t�@

	Camera camera;

	std::unique_ptr<NonSkinModel> model;
};
}