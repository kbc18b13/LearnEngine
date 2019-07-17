#include "stdafx.h"
#include "Camera.h"
#include "Graphic/GraphicEngine.h"
#include "Graphic/ConstBuffer/ConstBuffer.h"

namespace LearnEngine {

Camera::Camera() {}

Camera::~Camera() {}

void Camera::Apply(){
	ConstBuffer& cBuffer = Graphic().getCBuffer();
	cBuffer.setViewMatrix(getViewMat());
	cBuffer.setProjectionMatrix(getProjMat());
	cBuffer.UpdateCamera();
}

}