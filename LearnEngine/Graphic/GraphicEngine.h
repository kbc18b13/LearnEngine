#pragma once
#include "Camera/Camera.h"
#include "3DModel/TestBox.h"
#include "ConstBuffer/ConstBuffer.h"

namespace LearnEngine {
class NonSkinModel;

class GraphicEngine {
public:
	GraphicEngine();
	~GraphicEngine();

	void Init(HWND hWnd);

	//�`���ʂ̃��Z�b�g�B�`��O�ɌĂԁB
	void ClearRender();

	//���t���[���Ăԕ`��֐�
	void Render();

	ID3D11Device* getDevice() {
		return d3dDevice;
	}

	ID3D11DeviceContext* getContext() {
		return d3dContext;
	}

	ConstBuffer& getCBuffer() {
		return cBuffer;
	}

	Camera& getMainCamera() {
		return mainCamera;
	}

private:
	CComPtr<IDXGISwapChain>      swapChain;    //�X���b�v�`�F�C��
	CComPtr<ID3D11Device>        d3dDevice;    //d3d�f�o�C�X
	CComPtr<ID3D11DeviceContext> d3dContext;   //d3d�f�o�C�X�R���e�L�X�g
	D3D_FEATURE_LEVEL level;                   //DirectX�̑Ή����x��

	CComPtr<ID3D11Texture2D> depthStencil;
	CComPtr<ID3D11DepthStencilView> dsView;

	CComPtr <ID3D11RenderTargetView> targetView;//�����_�[�^�[�Q�b�g�r���[

	D3D11_VIEWPORT viewport;//�r���[�|�[�g

	ConstBuffer cBuffer;

	Camera mainCamera;
	Vector3 pos = {0,0,70};
	Vector3 up = { 0, 1, 0 };
	Vector3 mpos = { 0,-50,0 };

	std::unique_ptr<NonSkinModel> model;
	TestBox box;
};

//GraphicEngine���擾�BEngine.cpp�Ŏ�������Ă��܂��B
GraphicEngine& Graphic();

static inline ID3D11Device* D3DDevice() {
	return Graphic().getDevice();
};

static inline ID3D11DeviceContext* D3DContext() {
	return Graphic().getContext();
};

}