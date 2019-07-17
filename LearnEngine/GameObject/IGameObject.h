#pragma once
namespace LearnEngine {

class GameObjectManager;

class IGameObject {
public:
	virtual bool Start();
	virtual void Update();

	virtual void PostRender();

private:
	//ゲームオブジェクトマネージャーからprivateメンバをいじられる。
	friend class GameObjectManager;

	UINT goPriority;
	UINT goAddress;
	bool isDeleted;
};
}

