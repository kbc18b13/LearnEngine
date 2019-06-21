#pragma once
namespace LearnEngine {

class IGameObject;

class GameObjectManager {
public:
	GameObjectManager();
	~GameObjectManager();

	void AddGO(IGameObject* go);
	void DeleteGO(IGameObject* go);

	void Update();

private:
	std::vector<IGameObject*> goArray;
};

}
