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

	//毎フレーム呼ぶ描画関数
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

private:
	CComPtr<IDXGISwapChain>      swapChain;    //スワップチェイン
	CComPtr<ID3D11Device>        d3dDevice;    //d3dデバイス
	CComPtr<ID3D11DeviceContext> d3dContext;   //d3dデバイスコンテキスト
	D3D_FEATURE_LEVEL level;                   //DirectXの対応レベル

	CComPtr<ID3D11Texture2D> depthStencil;
	CComPtr<ID3D11DepthStencilView> dsView;

	CComPtr <ID3D11RenderTargetView> targetView;//レンダーターゲットビュー

	D3D11_VIEWPORT viewport;//ビューポート

	ConstBuffer cBuffer;

	Camera camera;
	Vector3 pos = {0,0,70};
	Vector3 up = { 0, 1, 0 };
	Vector3 mpos = { 0,-50,0 };

	std::unique_ptr<NonSkinModel> model;
	TestBox box;
};
}