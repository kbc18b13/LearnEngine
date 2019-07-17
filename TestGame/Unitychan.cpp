#include "stdafx.h"
#include "Unitychan.h"
#include "Graphic/3DModel/LoadCMO.h"

using namespace LE;

bool Unitychan::Start() {
	model = loadNonSkinModel("ModelData\\unityChan.cmo");
	return true;
}

void Unitychan::Update() {
	if (GetAsyncKeyState('T')) {
		mpos.y += 0.02f;
	}
	if (GetAsyncKeyState('G')) {
		mpos.y -= 0.02f;
	}
	if (GetAsyncKeyState('F')) {
		mpos.x += 0.02f;
	}
	if (GetAsyncKeyState('H')) {
		mpos.x -= 0.02f;
	}
	const Quaternion defoRot = Quaternion::GetRotationDeg(Vector3::AxisX(), -90.0f);
	model->UpdateWorldMatrix(mpos, defoRot);
}

void Unitychan::PostRender() {
	model->Draw();
}
