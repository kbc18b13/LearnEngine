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

	//ディレクションライトセット
	void setDirectionLight(const DirectionLight& dl) {
		light.directionLightVec = dl.getVec().normalize();
		light.directionLightColor = dl.getColor();
	}

	//ワールド行列セット
	void setWorldMatrix(const Matrix& mat) {
		object.worldMat = mat;
	}

	//ビュー行列セット
	void setViewMatrix(const Matrix& mat) {
		camera.viewMat = mat;
	}

	//プロジェクション行列セット
	void setProjectionMatrix(const Matrix& mat) {
		camera.projMat = mat;
	}


	//定数バッファはそれぞれのUpdateを呼ぶことで変更が適用される。
	void UpdateCamera();

	void UpdateLight();

	void UpdateObject();

private:
	enum {
		CAMERA_SLOT,
		LIGHT_SLOT,
		OBJECT_SLOT,
	};

	CComPtr<ID3D11Buffer> cameraCBuf;         //カメラの定数バッファ
	CComPtr<ID3D11Buffer> lightCBuf;          //ライトの定数バッファ
	CComPtr<ID3D11Buffer> objectCBuf;         //オブジェクトの定数バッファ

	CameraCB  camera;
	LightCB   light;
	ObjectCB  object;
};

}