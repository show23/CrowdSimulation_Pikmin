#pragma once

#include "Gameobject.h"
class BluePikmin : public GameObject
{
private:
	D3DXVECTOR3 m_Gravity;

	float m_Speed;//�Ǐ]���鑬�x�ɂ����Ă�l
	float upForce;
	float size;
	D3DXVECTOR3 sumX;
	float maxLength;//bullet��Player�̍ŒZ�����̂��̂�T�����߂̍X�V����l
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




	bool tracking;//Bullet��Player�ɂ��Ă��邩�ǂ����̐^�U
	bool groupTracking;//�s�N�~�����m���W�܂����Ƃ��̐^�U
	bool trackingFalse;//Tracking���U�ɂȂ��Ă�Ƃ��݂̂̐^�U
	bool notFollow;//�����^��ł�Ƃ��ɑ��̂��̂��^�ڂ��Ƃ��Ȃ�
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

