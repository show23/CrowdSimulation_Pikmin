#pragma once
#include "Gameobject.h"

class TitleLogo:public GameObject
{
private:

	ID3D11Buffer* m_VertexBuffer = NULL;
	ID3D11ShaderResourceView* m_Texture = NULL;
	ID3D11ShaderResourceView* m_Texture2 = NULL;
	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
	ID3D11InputLayout* m_VertexLayout;
	VERTEX_3D vertex[4];
	

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
	
};

