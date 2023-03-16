#pragma once

#include "Gameobject.h"
class Pikmin : public GameObject
{
private:
	D3DXVECTOR3 m_Gravity;
	
	float m_Speed;//追従する速度にかけてる値
	float upForce;
	float size;
	D3DXVECTOR3 sumX;
	
	float maxLength;//bulletとPlayerの最短距離のものを探すための更新する値
	char a;
	bool pikminFollow = false;
	bool dolphinStop = false;

	D3DXVECTOR3 normVec;
	

	D3DXVECTOR3 trackSpeed;
	D3DXVECTOR3 trackPikminSpeed;
	float vecSpeedx;
	float vecSpeedy;
	
	D3DXVECTOR3 vec;
	D3DXVECTOR3 sum;
	D3DXVECTOR3 avePosition;
	
	D3DXVECTOR3 m_TargetRotation;
	D3DXVECTOR3 pikminPosition;
	D3DXVECTOR3 pikminRotation;
	D3DXVECTOR3 pikminDirection;
	/*D3DXVECTOR3 bulletPosition = bullet->GetPosition();
	D3DXVECTOR3 bulletdirection = m_Position - bulletPosition;*/


	bool tracking;//BulletがPlayerについてくるかどうかの真偽
	bool groupTracking;//ピクミン同士が集まったときの真偽
	bool trackingFalse;//Trackingが偽になってるときのみの真偽
	bool notFollow;//物を運んでるときに他のものを運ぼうとしない
	static class Model* m_model;
	float count = 0;
	D3DXVECTOR3 trackingSpeed;
	//ベクトル
	float dx;
	float dy;
	float l;
	float x1;
	float y1;


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
