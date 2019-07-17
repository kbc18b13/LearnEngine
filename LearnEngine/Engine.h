#pragma once

namespace LearnEngine {

class GraphicEngine;
class GameObjectManager;

class GameEngine {
public:
	GameEngine();
	~GameEngine();

	//�v���O�����J�n����1�x�����K���ĂԂ��ƁB
	void InitEngine(HINSTANCE hInstance, int nCmdShow);

	int MessageLoop();

	GraphicEngine& getGraphic() {
		return *graphic;
	}

	GameObjectManager& getGOManager() {
		return *goManager;
	}

private:
	//�A�b�v�f�[�g�B�t���[���Ԃɍs�������̑S�Ă�S������B
	void Update();

	std::unique_ptr<GraphicEngine> graphic;
	std::unique_ptr<GameObjectManager> goManager;
};

GameEngine& Engine();

}



