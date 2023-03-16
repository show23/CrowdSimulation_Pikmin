#pragma once

#include "Gameobject.h"
class BluePikmin : public GameObject
{
private:
	D3DXVECTOR3 m_Gravity;

	float m_Speed;//追従する速度にかけてる値
	float upForce;
	float size;
	D3DXVECTOR3 sumX;
	float maxLength;//bulletとPlayerの最短距離のものを探すための更新する値
	bool bulletFollow = false;
	bool dolphinStop = false;

	D3DXVECTOR3 normVec;

	D3DXVECTOR3 trackSpeed;
	D3DXVECTOR3 trackBulletSpeed;
	float vecSpeedx;
	float vecSpeedy;
	
	
	D3DXVECTOR3 sum;
	D3DXVECTOR3 avePosition;

	D3DXVECTOR3 m_TargetRotation;
	D3DXVECTOR3 bluePikminPosition;
	D3DXVECTOR3 bluePikminRotation;
	D3DXVECTOR3 bluePikminDirection;
	/*D3DXVECTOR3 pikminPosition = bullet->GetPosition();
	D3DXVECTOR3 bulletdirection = m_Position - pikminPosition;*/




	bool tracking;//BulletがPlayerについてくるかどうかの真偽
	bool groupTracking;//ピクミン同士が集まったときの真偽
	bool trackingFalse;//Trackingが偽になってるときのみの真偽
	bool notFollow;//物を運んでるときに他のものを運ぼうとしない
	static class Model* m_model;
	float count = 0;
	D3DXVECTOR3 trackingSpeed;
	

	ID3D11VertexShader* m_VertexShader = NULL;
	ID3D11PixelShader* m_PixelShader = NULL;
	ID3D11InputLayout* m_VertexLayout = NULL;


	
public:
	void Init() override;
	void Uninit();
	void Update();
	void Draw();
	void Spawn();


	static void load();
	static void unload();
};

