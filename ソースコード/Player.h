#pragma once

#include "Gameobject.h"
class Player : public GameObject
{
private:
	
	class Model* m_model;
	class Audio* m_ShotSE;
	class Shadow* m_Shadow = nullptr;
	int m_Count;
	

	D3DXVECTOR3 m_Velocity;

	ID3D11VertexShader* m_VertexShader = NULL;
	ID3D11PixelShader* m_PixelShader = NULL;
	ID3D11InputLayout* m_VertexLayout = NULL;
	D3DXVECTOR3 playerLength;
	D3DXVECTOR3 m_TargetRotation;
	bool lineTrigger;//—\‘ªü”ÍˆÍ‚Ìíœ
	
public:
	void Init() override;
	void Uninit();
	void Update();
	void Draw();

	float MinLength = 1000.0f;
}; 
