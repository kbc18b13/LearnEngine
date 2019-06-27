#include "stdafx.h"
#include "GameObjectManager.h"
#include "IGameObject.h"

namespace LearnEngine {
GameObjectManager::GameObjectManager() {
}
GameObjectManager::~GameObjectManager() {
}
void GameObjectManager::Update() {
	for (IGameObject* go : goArray) {
		if (!go->isStarted) {
			go->Start();
		}
		go->Update();
	}

	for (int i = goArray.size(); i >= 0; i--) {
		if (goArray[i]->isDeleted) {
			delete goArray[i];
			//goArray.erase();
		}
	}
}
void GameObjectManager::AddGO(IGameObject* go) {
}
void GameObjectManager::DeleteGO(IGameObject* go) {
}
}