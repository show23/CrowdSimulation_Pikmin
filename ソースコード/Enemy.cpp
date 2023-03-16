#include "Main.h"
#include <Random>
#include "Renderer.h"
#include "Model.h"
#include "Manager.h"
#include "Scene.h"
#include "MeshField.h"
#include "Player.h"
#include "Pikmin.h"
#include "Enemy.h"

Model* Enemy::m_model;//staticのメンバ変数はcppで再度宣言をする


void Enemy::load()
{

	m_model = new Model();
	m_model->Load("asset\\model\\chappi-.obj");

}
void Enemy::unload()
{
	m_model->Unload();
	delete m_model;
}
void Enemy::Init()
{


	m_Position = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	//state == WAIT;
	attackCount = 0;

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "vertexLightingVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "vertexLightingPS.cso");
}

void Enemy::Uninit()
{
	
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void Enemy::Update()
{
	
	//メッシュフィールド適用
	MeshField* meshField = Manager::GetScene()->GetGameObject<MeshField>(1);
	m_Position.y = meshField->GetHeight(m_Position);
	
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//状態
	if (state == WAIT)
	{
		Wait();
	}
	else if (state == MOVE) {
		Move();
	}
	else if (state == CHASE) {
		
		Chase();
	}
	else if (state == ATTACK) {
		Attack();
	}
	
}

void Enemy::Draw()
{
	//入力レイアウト
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//ワールドマトリックス設定
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);

	world = scale * rot * trans;
	Renderer::SetWorldMatrix(&world);

	m_model->Draw();

}
//状態のコード
//待ち状態
void Enemy::Wait()
{
	Scene* scene = Manager::GetScene();
	//Player
	Player* player = scene->GetGameObject<Player>(1);
	D3DXVECTOR3 playerPosition = player->GetPosition();
	D3DXVECTOR3 playerRotation = player->GetRotation();
	D3DXVECTOR3 playerDirection = m_Position - playerPosition;//成分
	float playerLength = D3DXVec3Length(&playerDirection);//距離
	D3DXVECTOR3 playerNormVec = playerDirection / playerLength;//正規化
	float playerRadian = atan2(playerDirection.z, playerDirection.x);//角度

	m_Count++;
	//m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//float m_CountMin = 50;
	//if (m_Count < m_CountMin)
	//{
	//	searchCount++;//探してるように首を振ってほしい
	//	m_Rotation.y = -playerRadian - D3DX_PI + (searchCount / 100);
	//}
	//else
	//{
	//	searchCount--;
	//	m_Rotation.y = -playerRadian - D3DX_PI + (searchCount / 100);
	//}
	float m_CountMax = 100;
	if (m_Count >= m_CountMax)//m_Countが100になったらMOVEに移行
	{
		m_Count = 0;
		//searchCount = 0;
		
		state = MOVE;
	}
}
//歩く状態
void Enemy::Move()
{
	D3DXVECTOR3 forward = GetForward();
	Scene* scene = Manager::GetScene();

	//Player
	Player* player = scene->GetGameObject<Player>(1);
	D3DXVECTOR3 playerPosition = player->GetPosition();
	D3DXVECTOR3 playerRotation = player->GetRotation();
	D3DXVECTOR3 playerDirection = m_Position - playerPosition;//成分
	
	float playerLength = D3DXVec3Length(&playerDirection);//距離
	
	float playerRadian = atan2(playerDirection.z, playerDirection.x);//角度

	       
	
	//移動するポジション
	float movePos = -50.0f;
	D3DXVECTOR3 m_FirstPos = D3DXVECTOR3(movePos, 0.0f, 0.0f);//第一ポジション
	D3DXVECTOR3 m_SecondPos = D3DXVECTOR3(movePos,0.0f, -movePos);//第ニポジション
	D3DXVECTOR3 m_ThirdPos = D3DXVECTOR3(0.0f, 0.0f, -movePos);//第三ポジション
	D3DXVECTOR3 m_FourthPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//第四ポジション
	//距離の差
	D3DXVECTOR3 firstDirection = m_Position - m_FirstPos;
	D3DXVECTOR3 secondDirection = m_Position - m_SecondPos;
	D3DXVECTOR3 thirdDirection = m_Position - m_ThirdPos;
	D3DXVECTOR3 fourthDirection = m_Position - m_FourthPos;
	//角度
	float firstRadian = atan2(firstDirection.z, firstDirection.x);
	float secondRadian = atan2(secondDirection.z, secondDirection.x);
	float thirdRadian = atan2(thirdDirection.z, thirdDirection.x);
	float fourthRadian = atan2(fourthDirection.z, fourthDirection.x);
	//m_Rotation.y = thirdRadian - D3DX_PI;
	m_MoveCount++;
	float playerLengthMin = 20;

	
	if (playerLength > playerLengthMin)//EnemyとPlayerの距離がplayerLengthMin離れていると周回
	{
		
		float m_CountFirst = 300;
		float m_CountSecond = 600;
		float m_CountThird = 900;
		float m_CountFourth = 1200;
		float m_CountNone = 1500;

		float moveSpeed = 0.003f;
		//MOVEの時の移動
		if (m_MoveCount < m_CountFirst)
		{
			m_Rotation.y = -firstRadian - D3DX_PI;
			m_Position -= firstDirection * moveSpeed;
		}
		else if (m_MoveCount < m_CountSecond)
		{
			m_Rotation.y = -secondRadian - D3DX_PI;
			m_Position -= secondDirection * moveSpeed;
		}
		else if (m_MoveCount < m_CountThird)
		{
			m_Rotation.y = -thirdRadian - D3DX_PI;
			m_Position -= thirdDirection * moveSpeed;
		}
		else if (m_MoveCount < m_CountFourth)
		{
			m_Rotation.y = -fourthRadian - D3DX_PI;
			m_Position -= fourthDirection * moveSpeed;
		}
		else if (m_MoveCount < m_CountNone)
		{
			m_MoveCount = 0;
			state = WAIT;
		}
	}
	else//EnemyとPlayerの距離がplayerLengthMinより近いとCHASEに移行
	{
		state = CHASE;
	}
	
	
}

void Enemy::Chase()
{
	Scene* scene = Manager::GetScene();
	//Player
	Player* player = scene->GetGameObject<Player>(1);
	D3DXVECTOR3 playerPosition = player->GetPosition();
	D3DXVECTOR3 playerRotation = player->GetRotation();
	D3DXVECTOR3 playerDirection = m_Position - playerPosition;
	float playerLength = D3DXVec3Length(&playerDirection);
	D3DXVECTOR3 player_norm_vec = playerDirection / playerLength;

	float playerRadian = atan2(playerDirection.z, playerDirection.x);
	float playerLengthMax = 20;
	
	float attackRange = 17;

	
	if (playerLength > playerLengthMax)//playerLengthMaxより離れるとStateがWAIT
	{
		state = WAIT;
		
	}
	else//Playerを追いかける
	{
		float playerChase = 0.1f;
		m_Rotation.y = -playerRadian - D3DX_PI;
		m_Position -= player_norm_vec * playerChase;
		
		
	}
	if (playerLength < attackRange)
	{
		attackCount++;
		
		if (attackCount > 100)
		{
			state = ATTACK;
		}
		
	}
	else
	{
		attackCount = 0;
	}
}

void Enemy::Attack()
{
	Scene* scene = Manager::GetScene();
	//Player
	Player* player = scene->GetGameObject<Player>(1);
	D3DXVECTOR3 playerPosition = player->GetPosition();
	D3DXVECTOR3 playerRotation = player->GetRotation();
	D3DXVECTOR3 playerDirection = m_Position - playerPosition;
	float playerLength = D3DXVec3Length(&playerDirection);
	D3DXVECTOR3 player_norm_vec = playerDirection / playerLength;
	float playerRadian = atan2(playerDirection.z, playerDirection.x);
	
	D3DXVECTOR3 oldPlayerDirection = m_Position - m_OldPosition;
	float oldPlayerLength = D3DXVec3Length(&oldPlayerDirection);
	D3DXVECTOR3 old_player_norm_vec = oldPlayerDirection / oldPlayerLength;
	float oldPlayerRadian = atan2(oldPlayerDirection.z, oldPlayerDirection.x);
	float playerChase = 0.7f;
	float playerLengthMax = 20;
	
	m_Rotation.y = -oldPlayerRadian - D3DX_PI;
	waitTime++;
	if (waitTime < 90)
	{
		m_OldPosition = playerPosition;
	}
	else if(waitTime>100)
	{
		
		if (oldPlayerLength < 2)
		{
			m_Position -= old_player_norm_vec * 0;
		}
		else
		{
			m_Position -= old_player_norm_vec * playerChase;
		}
	}
	if(waitTime > 150)
	{
		state = WAIT;
		waitTime = 0;
	}
	
	
}
