#include "stdafx.h"
#include "GameObjectManager.h"
#include "IGameObject.h"
#include <algorithm>

namespace LearnEngine {
GameObjectManager::GameObjectManager() {
}
GameObjectManager::~GameObjectManager() {
}

void GameObjectManager::Init(UINT defMaxPriority, UINT defGoCapacity) {
	goArray.resize(defMaxPriority);
	defaultGoCapacity = defGoCapacity;
	for (UINT i = 0; i < defMaxPriority; i++) {
		goArray[i].reserve(defGoCapacity);
	}
}

bool GameObjectManager::GoPriorityComp(IGameObject* a, IGameObject* b) {
	return (a->goPriority) < (b->goPriority);
}

void GameObjectManager::Update() {

	//スタート関数の呼び出し
	std::sort(startGOArray.begin(), startGOArray.end(), GoPriorityComp);
	for (int i = startGOArray.size() - 1; i >= 0; i--) {
		startGOArray[i]->Start();
		startGOArray.pop_back();
	}

	//アップデート関数の呼び出し。
	for (std::vector<IGameObject*>& goPriorityArray : goArray) {
		for (IGameObject* go : goPriorityArray) {
			go->Update();
		}
	}

	//ポストレンダー関数の呼び出し。
	for (std::vector<IGameObject*>& goPriorityArray : goArray) {
		for (IGameObject* go : goPriorityArray) {
			go->PostRender();
		}
	}

	//DeleteGOされてしまった奴らのdeleteの呼び出し
	std::sort(deleteGOArray.begin(), deleteGOArray.end(), GoPriorityComp);
	for (int i = deleteGOArray.size()-1; i >= 0; i--) {
		delete deleteGOArray[i];
		deleteGOArray.pop_back();
	}
}

void GameObjectManager::AddGO(IGameObject* go, UINT priority) {
	//新しい優先順位がやってきた場合、goArrayをリサイズして許容量も設定する。
	size_t priSize = goArray.size();
	if (priSize <= priority) {
		goArray.resize(priority + 1);
		for (UINT i = priSize; i <= priority; i++) {
			goArray[i].reserve(defaultGoCapacity);
		}
	}
	//Start待機配列とgoArrayにプッシュバック。
	startGOArray.push_back(go);
	goArray[priority].push_back(go);
}

void GameObjectManager::DeleteGO(IGameObject* go) {
	RemoveGO(go);
	deleteGOArray.push_back(go);
}

void GameObjectManager::RemoveGO(IGameObject* go) {
	if (1 < goArray.size()) {
		goArray[go->goPriority][go->goAddress] = goArray[go->goPriority].back();
	}
	goArray[go->goPriority].pop_back();
}
}