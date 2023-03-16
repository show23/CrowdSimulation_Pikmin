#pragma once

#include "Gameobject.h"
class Platform : public GameObject
{
private:

	static class Model* m_model;


	ID3D11VertexShader* m_VertexShader = NULL;
	ID3D11PixelShader* m_PixelShader = NULL;
	ID3D11InputLayout* m_VertexLayout = NULL;

	bool platformFollow = false;
	bool platformPikuminFollow = false;
	float count = 0;
	float m_Count = 0;
	bool once;

public:
	void Init() override;
	void Uninit();
	void Update();
	void Draw();
	static void load();
	static void unload();
};
