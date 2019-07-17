#pragma once
namespace LearnEngine {

class IGameObject;

class GameObjectManager {
public:
	GameObjectManager();
	~GameObjectManager();

	void Init(UINT defaultMaxPriority, UINT defaultGoCapacity);

	void AddGO(IGameObject* go, UINT priority);

	void DeleteGO(IGameObject* go);

	void RemoveGO(IGameObject* go);

	void Update();

private:
	//ゲームオブジェクトの優先順位を比較する関数。
	//ソートに使用すると優先度が低い順に並び替えます。
	//vectorでケツから見て行って捨てていくためです。
	static bool GoPriorityComp(IGameObject* a, IGameObject* b);

	UINT defaultGoCapacity = 10;
	std::vector<IGameObject*> startGOArray;
	std::vector<IGameObject*> deleteGOArray;
	std::vector<std::vector<IGameObject*>> goArray;
};

}
