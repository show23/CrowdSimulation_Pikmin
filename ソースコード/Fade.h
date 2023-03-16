#pragma once
#include "Gameobject.h"
class Fade :public GameObject
{
private:
	ID3D11Buffer* m_VertexBuffer = NULL;
	ID3D11ShaderResourceView* m_Texture = NULL;

	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
	ID3D11InputLayout* m_VertexLayout;

	float m_Count;

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
};