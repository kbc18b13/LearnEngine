#pragma once
namespace LearnEngine {

class GameObjectManager;

class IGameObject {
public:
	virtual bool Start();
	virtual void Update();

	virtual void PostRender();

private:
	//�Q�[���I�u�W�F�N�g�}�l�[�W���[����private�����o����������B
	friend class GameObjectManager;

	UINT goPriority;
	UINT goAddress;
	bool isDeleted;
};
}

