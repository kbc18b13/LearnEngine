#pragma once
#include "Graphic/Light/DirectionLight.h"

namespace LearnEngine {

struct CameraCB {
	Matrix viewMat;
	Matrix projMat;
};

struct LightCB {
	Vector4 directionLightColor;
	Vector3 directionLightVec;
private:
	float dummy;
};

struct ObjectCB {
	Matrix worldMat;
};

class ConstBuffer {
public:
	ConstBuffer();
	~ConstBuffer();

	void Init();

	//�f�B���N�V�������C�g�Z�b�g
	void setDirectionLight(const DirectionLight& dl) {
		light.directionLightVec = dl.getVec().normalize();
		light.directionLightColor = dl.getColor();
	}

	//���[���h�s��Z�b�g
	void setWorldMatrix(const Matrix& mat) {
		object.worldMat = mat;
	}

	//�r���[�s��Z�b�g
	void setViewMatrix(const Matrix& mat) {
		camera.viewMat = mat;
	}

	//�v���W�F�N�V�����s��Z�b�g
	void setProjectionMatrix(const Matrix& mat) {
		camera.projMat = mat;
	}


	//�萔�o�b�t�@�͂��ꂼ���Update���ĂԂ��ƂŕύX���K�p�����B
	void UpdateCamera();

	void UpdateLight();

	void UpdateObject();

private:
	enum {
		CAMERA_SLOT,
		LIGHT_SLOT,
		OBJECT_SLOT,
	};

	CComPtr<ID3D11Buffer> cameraCBuf;         //�J�����̒萔�o�b�t�@
	CComPtr<ID3D11Buffer> lightCBuf;          //���C�g�̒萔�o�b�t�@
	CComPtr<ID3D11Buffer> objectCBuf;         //�I�u�W�F�N�g�̒萔�o�b�t�@

	CameraCB  camera;
	LightCB   light;
	ObjectCB  object;
};

}