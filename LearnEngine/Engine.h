#pragma once

namespace LearnEngine {

class GraphicEngine;
class GameObjectManager;

class GameEngine {
public:
	GameEngine();
	~GameEngine();

	//プログラム開始時に1度だけ必ず呼ぶこと。
	void InitEngine(HINSTANCE hInstance, int nCmdShow);

	int MessageLoop();

	GraphicEngine& getGraphic() {
		return *graphic;
	}

	GameObjectManager& getGOManager() {
		return *goManager;
	}

private:
	//アップデート。フレーム間に行う処理の全てを担当する。
	void Update();

	std::unique_ptr<GraphicEngine> graphic;
	std::unique_ptr<GameObjectManager> goManager;
};

GameEngine& Engine();

}



