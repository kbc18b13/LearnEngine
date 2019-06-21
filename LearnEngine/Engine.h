#pragma once
#include "Graphic/GraphicEngine.h"

namespace LearnEngine {

class GameEngine {
public:
	void InitEngine(HINSTANCE hInstance, int nCmdShow);

	int MessageLoop();

	GraphicEngine& getGraphic() {
		return graphic;
	}

private:
	GraphicEngine graphic;
};

GameEngine& Engine();

ID3D11Device* D3DDevice() {
	return Engine().getGraphic().getDevice();
}

ID3D11DeviceContext* D3DContext() {
	return Engine().getGraphic().getContext();
}

}



