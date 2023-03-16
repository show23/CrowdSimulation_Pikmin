#pragma once


#include "Scene.h"

class Game :public Scene
{
private:
	bool m_Destroy = false;
	float m_FadeCount;
	bool start;
public:

	void SetDestroy() { m_Destroy = true; }
	void Init();
	void Uninit();
	void Update();

	bool Destroy()
	{
		if (m_Destroy)
		{
			Uninit();
			delete this;
			return true;
		}
		else
		{
			return false;
		}
	}
};