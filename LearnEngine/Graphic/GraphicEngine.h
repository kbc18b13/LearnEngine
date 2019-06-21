namespace LearnEngine {
class GraphicEngine {
public:
	void Init(HWND hWnd);

	void Render();

	ID3D11Device* getDevice() {
		return d3dDevice;
	}

	ID3D11DeviceContext* getContext() {
		return d3dContext;
	}

private:
	CComPtr<IDXGISwapChain>      swapChain;    //スワップチェイン
	CComPtr<ID3D11Device>        d3dDevice;    //d3dデバイス
	CComPtr<ID3D11DeviceContext> d3dContext;   //d3dデバイスコンテキスト
	D3D_FEATURE_LEVEL level;                   //DirectXの対応レベル

	CComPtr<ID3D11Texture2D> depthStencil;
	CComPtr<ID3D11DepthStencilView> dsView;

	CComPtr <ID3D11InputLayout> inputLayout;

	CComPtr <ID3D11RenderTargetView> targetView;//レンダーターゲットビュー
};
}