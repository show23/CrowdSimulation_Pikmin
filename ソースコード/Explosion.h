#pragma once
#include "Gameobject.h"
class EXPLOSION : public GameObject
{
private:


	ID3D11Buffer* m_VertexBuffer = NULL;
	ID3D11ShaderResourceView* m_Texture = NULL;

	ID3D11VertexShader* m_VertexShader = NULL;
	ID3D11PixelShader* m_PixelShader = NULL;
	ID3D11InputLayout* m_VertexLayout = NULL;

	int m_Count;
	int texMax = 4;
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
};