#pragma once
#include "Graphic/3DModel/NonSkinModel.h"

class Unitychan : public LE::IGameObject{
public:
	bool Start() override;
	void Update() override;

	void PostRender() override;

private:
	std::unique_ptr<LE::NonSkinModel> model;
	LE::Vector3 mpos = { 0,-50,0 };
};

