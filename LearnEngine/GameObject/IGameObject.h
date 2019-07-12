#pragma once
namespace LearnEngine {

class GameObjectManager;

class IGameObject {
public:
	bool Start();
	void Update();

private:
	UINT goPriority;
	UINT goAddress;

	friend class GameObjectManager;
};
}

