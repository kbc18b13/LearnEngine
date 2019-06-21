#include "..\\LearnEngine\Engine.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
					  _In_opt_ HINSTANCE /*hPrevInstance*/,
					  _In_ LPWSTR    /*lpCmdLine*/,
					  _In_ int       nCmdShow){
	using namespace LearnEngine;

	Engine().InitEngine(hInstance, nCmdShow);

	return Engine().MessageLoop();
}