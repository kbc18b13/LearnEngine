#pragma once

//エンジンを使う上でグローバルに使いたい関数群。
//Engine.cppで実装されています。

namespace LearnEngine{

class IGameObject;

//ゲームオブジェクトを作成し、マネージャーに登録する。
template<typename T>
T* NewGO(UINT priority) {
	T* go = new T();
	AddGO(go, priority);
	return go;
}

//ゲームオブジェクトを作成し、マネージャーに登録する。
//initArgsに渡した引数でコンストラクタを呼ぶ。
template<typename T, typename ...InitializeArgs>
T* NewGO(UINT priority, InitializeArgs ... initArgs) {
	T* go = new T(initArgs);
	AddGO(go, priority);
}

//ゲームオブジェクトをマネージャーに登録
void AddGO(IGameObject* go, UINT priority);

//ゲームオブジェクトをマネージャーから削除したのち、deleteする
void DeleteGO(IGameObject* go);

//ゲームオブジェクトをマネージャーから削除
void RemoveGO(IGameObject* go);

//メインカメラを取得
class Camera;
Camera& MainCamera();

}