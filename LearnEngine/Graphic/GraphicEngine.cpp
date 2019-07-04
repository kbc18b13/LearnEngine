#include "GraphicEngine.h"
#include "..\\stdafx.h"
#include "3DModel/NonSkinModel.h"
#include "3DModel/LoadCMO.h"

namespace LearnEngine {
GraphicEngine::GraphicEngine() {
}
GraphicEngine::~GraphicEngine() {
}

void GraphicEngine::Init(HWND hwnd) {
	//DirectXの初期化
	RECT rect;
	GetClientRect(hwnd, &rect);
	DXGI_SWAP_CHAIN_DESC scDesc = {};//スワップチェインのもろもろの設定を入れる構造体
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
		throw "Filed create Device";//directXの初期化に失敗した場合
	}

	//スワップチェイン
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

	//ビューポートの設定
	viewport.Height = rect.bottom - rect.top;
	viewport.Width = rect.right - rect.left;
	viewport.MaxDepth = 1.0f;
	viewport.MinDepth = 0.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	//深度ステンシルバッファ
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

	{//ラスタライザ設定。カリング設定したかっただけ。
		D3D11_RASTERIZER_DESC rsDesc{};
		rsDesc.CullMode = D3D11_CULL_NONE;
		rsDesc.FillMode = D3D11_FILL_SOLID;
		rsDesc.FrontCounterClockwise = true;
		CComPtr<ID3D11RasterizerState> rsState;
		result = d3dDevice->CreateRasterizerState(&rsDesc, &rsState);
		if (FAILED(result)) {
			abort();
		}
		d3dContext->RSSetState(rsState);
	}

	model = loadNonSkinModel("ModelData\\box.cmo");
	camera.setFar(300.0f);
	camera.setNear(1.0f);
	camera.setAspect(16.0f / 9);
	camera.setFOV(DegToRad(90.0f));

	camera.setLook({ 0, 0, 0 });
	camera.setPos(pos);
	camera.setUp({ 0, 1, 0 });

	D3D11_BUFFER_DESC cBufDesc{};
	cBufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cBufDesc.ByteWidth = sizeof(Matrix);
	cBufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cBufDesc.Usage = D3D11_USAGE_DYNAMIC;

	D3D11_SUBRESOURCE_DATA cBuf{};
	Matrix proj = camera.getProjMat();
	cBuf.pSysMem = &proj;
	result = d3dDevice->CreateBuffer(&cBufDesc, &cBuf, &projCBuf);
	if (FAILED(result)) {
		abort();
	}

	Matrix view = camera.getViewMat();
	cBuf.pSysMem = &view;
	result = d3dDevice->CreateBuffer(&cBufDesc, &cBuf, &viewCBuf);
	if (FAILED(result)) {
		abort();
	}

	box.Init();
}

void GraphicEngine::Render() {
	if (GetAsyncKeyState('A')) {
		pos.x += 0.1f;
	}
	if (GetAsyncKeyState('D')) {
		pos.x -= 0.1f;
	}
	if (GetAsyncKeyState('W')) {
		pos.z += 0.1f;
	}
	if (GetAsyncKeyState('S')) {
		pos.z -= 0.1f;
	}
	camera.setPos(pos);

	{
		D3D11_MAPPED_SUBRESOURCE mapRes;
		HRESULT res = d3dContext->Map(viewCBuf, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapRes);
		if (FAILED(res)) {
			abort();
		}
		*(static_cast<Matrix*>(mapRes.pData)) = camera.getViewMat();
		d3dContext->Unmap(viewCBuf, 0);
	}

	d3dContext->VSSetConstantBuffers(0, 1, &projCBuf.p);
	d3dContext->VSSetConstantBuffers(1, 1, &viewCBuf.p);

	d3dContext->OMSetRenderTargets(1, &targetView.p, dsView);
	d3dContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	d3dContext->RSSetViewports(1, &viewport);
	float color[4] = { 0.5f,0.5f,0.5f,1 };
	d3dContext->ClearRenderTargetView(targetView, color);
	d3dContext->ClearDepthStencilView(dsView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	box.Draw();

	swapChain->Present(0, 0);
}

}