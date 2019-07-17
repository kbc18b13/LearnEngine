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
	//�Q�[���I�u�W�F�N�g�̗D�揇�ʂ��r����֐��B
	//�\�[�g�Ɏg�p����ƗD��x���Ⴂ���ɕ��ёւ��܂��B
	//vector�ŃP�c���猩�čs���Ď̂ĂĂ������߂ł��B
	static bool GoPriorityComp(IGameObject* a, IGameObject* b);

	UINT defaultGoCapacity = 10;
	std::vector<IGameObject*> startGOArray;
	std::vector<IGameObject*> deleteGOArray;
	std::vector<std::vector<IGameObject*>> goArray;
};

}
