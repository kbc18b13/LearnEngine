#include "stdafx.h"
#include "Camera.h"
#include "Engine.h"
#include "Graphic/ConstBuffer/ConstBuffer.h"

namespace LearnEngine {

Camera::Camera() {}

Camera::~Camera() {}

void Camera::Apply(){
	ConstBuffer& cBuffer = Engine().getGraphic().getCBuffer();
	cBuffer.setViewMatrix(getViewMat());
	cBuffer.setProjectionMatrix(getProjMat());
	cBuffer.UpdateCamera();
}

}