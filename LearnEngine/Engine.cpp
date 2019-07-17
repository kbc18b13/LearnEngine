#include "Engine.h"
#include "EngineFunc.h"
#include "Resource/resource.h"

#include "Graphic/GraphicEngine.h"
#include "GameObject/GameObjectManager.h"

namespace LearnEngine {

////////���b�Z�[�W�v���V�[�W���ƃG���W���̃����o�֐�////////////////////////

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

GameEngine::GameEngine() : graphic(new GraphicEngine()), goManager(new GameObjectManager()){
}

GameEngine::~GameEngine() {
}

void GameEngine::InitEngine(HINSTANCE hInstance, int nCmdShow) {
	//�J�����g�f�B���N�g����Resource�t�H���_��
	if (!SetCurrentDirectoryW(L"Resource")) {
		abort();
	}

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

	//�O���t�B�b�N�G���W��������
	graphic->Init(hWnd);

	//�Q�[���I�u�W�F�N�g�}�l�[�W���[������
	goManager->Init(5, 10);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
}

int GameEngine::MessageLoop() {
	MSG msg;

	// ���C�� ���b�Z�[�W ���[�v:
	while (true) {
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				break;
			}
			DispatchMessage(&msg);
		}
		Update();
	}

	return (int)msg.wParam;
}

//private�֐�
void GameEngine::Update() {
	graphic->ClearRender();

	goManager->Update();

	graphic->Render();
}



///////////�O���[�o���֐��̒�`///////////////////

static GameEngine g_engine;

//�G���W�����擾
GameEngine& Engine() {
	return g_engine;
}

//�O���t�B�b�N�G���W�����擾
GraphicEngine& Graphic() {
	return g_engine.getGraphic();
}

//�Q�[���I�u�W�F�N�g���}�l�[�W���[�ɓo�^
void AddGO(IGameObject* go, UINT priority) {
	return g_engine.getGOManager().AddGO(go, priority);
}

//�Q�[���I�u�W�F�N�g���}�l�[�W���[����폜�����̂��Adelete����
void DeleteGO(IGameObject* go) {
	return g_engine.getGOManager().DeleteGO(go);
}


//�Q�[���I�u�W�F�N�g���}�l�[�W���[����폜
void RemoveGO(IGameObject* go) {
	return g_engine.getGOManager().RemoveGO(go);
}

Camera& MainCamera() {
	return g_engine.getGraphic().getMainCamera();
}

}
