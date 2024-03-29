#pragma once

#include <stdio.h>
#include "Gameobject.h"
class Enemy : public GameObject
{
private:

	static class Model* m_model;


	ID3D11VertexShader* m_VertexShader = NULL;
	ID3D11PixelShader* m_PixelShader = NULL;
	ID3D11InputLayout* m_VertexLayout = NULL;
	D3DXVECTOR3 m_TargetRotation;
	
	float m_Count;//WAITの時の待ち時間
	float searchCount;//WAITのときの首振り
	float attackCount;//攻撃待機時間
	float waitTime;
	float m_MoveCount;
	D3DXVECTOR3 m_OldPosition;
	bool move;
	bool first;
	bool second;
	bool third;
	bool fourth;
	bool attack;

	D3DXVECTOR3 m_FirstPos;
	D3DXVECTOR3 m_SecondPos;
	D3DXVECTOR3 m_ThirdPos;
	D3DXVECTOR3 m_FourthPos;

	//状態
	enum State {
		WAIT,
		MOVE,
		CHASE,
		ATTACK,
	};
	State state;
	void Wait();
	void Move();
	void Chase();
	void Attack();


public:
	void Init() override;
	void Uninit();
	void Update();
	void Draw();
	static void load();
	static void unload();

};
