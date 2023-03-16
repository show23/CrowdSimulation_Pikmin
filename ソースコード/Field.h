#pragma once
#include "Gameobject.h"
class FIELD : public GameObject
{
private:
	
	VERTEX_3D vertex[4];
	ID3D11Buffer* m_VertexBuffer = NULL;
	ID3D11ShaderResourceView* m_Texture = NULL;

	ID3D11VertexShader* m_VertexShader = NULL;
	ID3D11PixelShader* m_PixelShader = NULL;
	ID3D11InputLayout* m_VertexLayout = NULL;
	float m_Count;
	float X = -30.0f;
	float Y = 100;;
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
};