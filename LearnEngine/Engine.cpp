#include "Engine.h"
#include "Resource/resource.h"

namespace LearnEngine {

static GameEngine g_engine;

GameEngine& Engine() {
	return g_engine;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void GameEngine::InitEngine(HINSTANCE hInstance, int nCmdShow) {
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = nullptr;
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = L"LearnGameEngine";
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON_SMALL));

	RegisterClassExW(&wcex);

	HWND hWnd = CreateWindowW(L"LearnGameEngine", L"LearnGameEngine", WS_OVERLAPPEDWINDOW,
							  CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd) {

	}

	graphic.Init(hWnd);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
}

int GameEngine::MessageLoop() {
	MSG msg;

	// メイン メッセージ ループ:
	while (true) {
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				break;
			}
			DispatchMessage(&msg);
		}
		graphic.Render();
	}

	return (int)msg.wParam;
}

}
