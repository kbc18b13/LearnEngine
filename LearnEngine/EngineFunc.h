#pragma once

//�G���W�����g����ŃO���[�o���Ɏg�������֐��Q�B
//Engine.cpp�Ŏ�������Ă��܂��B

namespace LearnEngine{

class IGameObject;

//�Q�[���I�u�W�F�N�g���쐬���A�}�l�[�W���[�ɓo�^����B
template<typename T>
T* NewGO(UINT priority) {
	T* go = new T();
	AddGO(go, priority);
	return go;
}

//�Q�[���I�u�W�F�N�g���쐬���A�}�l�[�W���[�ɓo�^����B
//initArgs�ɓn���������ŃR���X�g���N�^���ĂԁB
template<typename T, typename ...InitializeArgs>
T* NewGO(UINT priority, InitializeArgs ... initArgs) {
	T* go = new T(initArgs);
	AddGO(go, priority);
}

//�Q�[���I�u�W�F�N�g���}�l�[�W���[�ɓo�^
void AddGO(IGameObject* go, UINT priority);

//�Q�[���I�u�W�F�N�g���}�l�[�W���[����폜�����̂��Adelete����
void DeleteGO(IGameObject* go);

//�Q�[���I�u�W�F�N�g���}�l�[�W���[����폜
void RemoveGO(IGameObject* go);

//���C���J�������擾
class Camera;
Camera& MainCamera();

}