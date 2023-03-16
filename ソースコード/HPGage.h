#pragma once
#include "Gameobject.h"

class HPGage : public GameObject
{

private:


	ID3D11Buffer* m_VertexBuffer = NULL;
	ID3D11ShaderResourceView* m_Texture = NULL;

	ID3D11VertexShader* m_VertexShader = NULL;
	ID3D11PixelShader* m_PixelShader = NULL;
	ID3D11InputLayout* m_VertexLayout = NULL;

    float m_Count = 1.0f;
	float x;
	float flowerCount;

	bool flowerGeneretor = false;
	bool m_HPGageDestroy = false;

	
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	void SetHpGage() { m_HPGageDestroy = true; }

};

