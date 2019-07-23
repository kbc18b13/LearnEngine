#include "GraphicEngine.h"
#include "..\\stdafx.h"
#include "3DModel/NonSkinModel.h"
#include "3DModel/LoadCMO.h"
#include "Math/Quaternion.h"

namespace LearnEngine {
GraphicEngine::GraphicEngine() {
}
GraphicEngine::~GraphicEngine() {
}

void GraphicEngine::Init(HWND hwnd) {
	//DirectX�̏�����
	RECT rect;
	GetClientRect(hwnd, &rect);
	DXGI_SWAP_CHAIN_DESC scDesc = {};//�X���b�v�`�F�C���̂������̐ݒ������\����
	scDesc.BufferCount = 1;
	scDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scDesc.BufferDesc.Height = rect.bottom - rect.top;
	scDesc.BufferDesc.Width = rect.right - rect.left;
	scDesc.BufferDesc.RefreshRate.Numerator = 60;
	scDesc.BufferDesc.RefreshRate.Numerator = 1;
	scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scDesc.OutputWindow = hwnd;
	scDesc.SampleDesc.Count = 1;
	scDesc.SampleDesc.Quality = 0;
	scDesc.Windowed = true;

	HRESULT result = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		D3D11_CREATE_DEVICE_SINGLETHREADED ||
#ifdef _DEBUG
		D3D11_CREATE_DEVICE_DEBUG
#endif
		, nullptr,
		0,
		D3D11_SDK_VERSION,
		&scDesc,
		&swapChain,
		&d3dDevice,
		&level,
		&d3dContext
	);
	if (FAILED(result)) {
		throw "Filed create Device";//directX�̏������Ɏ��s�����ꍇ
	}

	//�X���b�v�`�F�C��
	{
		CComPtr<ID3D11Texture2D> pBackBuffer;
		result = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)& pBackBuffer);
		if (FAILED(result)) {
			throw "Filed create BackBuffer";
		}
		result = d3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &targetView);
		if (FAILED(result)) {
			throw "Filed create RenderTargetView";
		}
	}

	//�r���[�|�[�g�̐ݒ�
	viewport.Height = static_cast<float>(rect.bottom - rect.top);
	viewport.Width = static_cast<float>(rect.right - rect.left);
	viewport.MaxDepth = 1.0f;
	viewport.MinDepth = 0.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	//�[�x�X�e���V���o�b�t�@
	D3D11_TEXTURE2D_DESC texDesc = {};
	texDesc.Width = scDesc.BufferDesc.Width;
	texDesc.Height = scDesc.BufferDesc.Height;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;
	result = d3dDevice->CreateTexture2D(&texDesc, nullptr, &depthStencil);
	if (FAILED(result)) {
		throw "Filed create DepthStencilTexture";
	}

	D3D11_DEPTH_STENCIL_VIEW_DESC dsDesc = {};
	dsDesc.Format = texDesc.Format;
	dsDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsDesc.Texture2D.MipSlice = 0;
	result = d3dDevice->CreateDepthStencilView(depthStencil, &dsDesc, &dsView);
	if (FAILED(result)) {
		throw "Filed create DepthStencilView";
	}

	{//���X�^���C�U�ݒ�B�J�����O�ݒ肵�������������B
		D3D11_RASTERIZER_DESC rsDesc{};
		rsDesc.CullMode = D3D11_CULL_BACK;
		rsDesc.FillMode = D3D11_FILL_SOLID;
		rsDesc.FrontCounterClockwise = false;
		CComPtr<ID3D11RasterizerState> rsState;
		result = d3dDevice->CreateRasterizerState(&rsDesc, &rsState);
		if (FAILED(result)) {
			abort();
		}
		d3dContext->RSSetState(rsState);
	}

	////�����p���f��
	//model = loadNonSkinModel("ModelData\\unityChan.cmo");

	//�萔�o�b�t�@�̏�����
	cBuffer.Init();

	//�����p�{�b�N�X
	box.Init();
	{
		DirectionLight light;
		light.setColor({ 1,1,1,1 });
		light.setVec({ 0,-1,0 });
		cBuffer.setDirectionLight(light);
		cBuffer.UpdateLight();
	}
}

void GraphicEngine::ClearRender() {
	d3dContext->OMSetRenderTargets(1, &targetView.p, dsView);
	d3dContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	d3dContext->RSSetViewports(1, &viewport);
	float color[4] = { 0.5f,0.5f,0.5f,1 };
	d3dContext->ClearRenderTargetView(targetView, color);
	d3dContext->ClearDepthStencilView(dsView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	mainCamera.Apply();
}

void GraphicEngine::Render() {

	//box.Draw();
	//model->Draw();
	swapChain->Present(0, 0);
}

}