#pragma once
class CCamera : public LE::IGameObject{
public:
	bool Start();
	void Update();

private:
	LE::Vector3 pos = { 0,0,70 };
	LE::Vector3 up = { 0, 1, 0 };
};

