#pragma once
#include "Gameobject.h"
class TitleStartButton :public GameObject
{
private:
	ID3D11Buffer* m_VertexBuffer = NULL;
	ID3D11ShaderResourceView* m_Texture = NULL;
	ID3D11ShaderResourceView* m_Texture2 = NULL;
	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
	ID3D11InputLayout* m_VertexLayout;
	VERTEX_3D vertex[4];
	VERTEX_3D vertex2[4];

	float Alpha = 1.0f;
	
	float m_Count = 0.0f;
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
};
