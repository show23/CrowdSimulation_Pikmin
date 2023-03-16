#pragma once
#include "Scene.h"

class Title :public Scene
{
private:
	float m_TitleCount;
	bool start;
	float m_Count;

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
};