#include "..\\LearnEngine\Engine.h"
#include "CCamera.h"
#include "Unitychan.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
					  _In_opt_ HINSTANCE /*hPrevInstance*/,
					  _In_ LPWSTR    /*lpCmdLine*/,
					  _In_ int       nCmdShow){
	using namespace LearnEngine;

	Engine().InitEngine(hInstance, nCmdShow);

	NewGO<CCamera>(0);
	NewGO<Unitychan>(0);

	return Engine().MessageLoop();
}