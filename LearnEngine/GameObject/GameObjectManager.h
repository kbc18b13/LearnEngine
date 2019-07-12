#pragma once
namespace LearnEngine {

class IGameObject;

struct PriorityGo {
	PriorityGo(IGameObject* g, UINT p) : goPtr(g), priority(p) {}

	bool operator<(const PriorityGo& pg) {
		return priority < pg.priority;
	}

	IGameObject* goPtr;
	UINT priority;
};

class GameObjectManager {
public:
	GameObjectManager();
	~GameObjectManager();

	void Init(UINT defaultMaxPriority, UINT defaultGoCapacity);

	void AddGO(IGameObject* go, UINT priority);
	void DeleteGO(IGameObject* go);

	void RemoveGO(UINT priority, USHORT address);

	void Update();

private:
	static bool GoPriorityComp(IGameObject* a, IGameObject* b);

	UINT defaultGoCapacity = 10;
	std::vector<IGameObject*> startGOArray;
	std::vector<IGameObject*> deleteGOArray;
	std::vector<std::vector<IGameObject*>> goArray;
};

}
