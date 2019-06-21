#pragma once
namespace LearnEngine {
class IGameObject {
public:
	bool Start();
	void Update();

	bool isStarted = false;
	bool isDeleted = false;
};
}

