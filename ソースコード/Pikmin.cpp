
#include <iostream>
#include "Main.h"
#include <cmath>
#include <algorithm>
#include <time.h>
#include <math.h>
#include "Manager.h"
#include "Renderer.h"
#include "Scene.h"
#include "Input.h"
#include "Player.h"
#include "Model.h"
#include "Pikmin.h"
#include "Line.h"
#include "SetRing.h"
#include "Explosion.h"
#include "Enemy.h"
#include "Platform.h"
#include "MeshField.h"
#include "One.h"
#include "Dolphin.h"
#include <random>

Model* Pikmin::m_model;//staticのメンバ変数はcppで再度宣言をする
void Pikmin::load()
{
	//モデルのセット
	m_model = new Model();
	m_model->Load("asset\\model\\Pikumin1018.obj");
}

void Pikmin::unload()
{
	//モデルのアンロード
	m_model->Unload();
	delete m_model;
}


void Pikmin::Init()
{
	
	m_Position = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(0.2f, 0.2f, 0.2f);
	m_Gravity = D3DXVECTOR3(0.0f, 0.005f, 0.0f);
	trackingSpeed = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//bulletがPlayerについてくるスピード
	trackSpeed = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//中心に集まる速度
	trackPikminSpeed = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//Bulletどうしの当たり判定に使われている反発の値などを入れてる
	normVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//Bullet同士の当たり判定の反発（ノーマライズしてある＊データのスケールを扱いやすいものに整えること）
	m_Velocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Speed = 0.001f;//追従する速度にかけてる値
	upForce = 0.2f;//ジャンプする速度（今は使っていない）

	
	
	maxLength = 10000.0f;//bulletとPlayerの最短距離のものを探すための更新する値
	
	vecSpeedx = 0.0f;
	vecSpeedy = 0.0f;
	m_TargetRotation = D3DXVECTOR3(0, 0, 0);
	
	tracking = false;//BulletがPlayerについてくるかどうかの真偽
	trackingFalse = false;//Trackingが偽になってるときのみの真偽
	groupTracking = false;//ピクミン同士が集まったときの真偽
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "vertexLightingVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "vertexLightingPS.cso");
	
	
}

void Pikmin::Uninit()
{
	

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void Pikmin::Update()
{
	D3DXVECTOR3 forward = GetForward();

	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>(1);
	D3DXVECTOR3 playerPosition = player->GetPosition();
	D3DXVECTOR3 playerRotation = player->GetRotation();
	D3DXVECTOR3 playerDirection = m_Position - playerPosition;//成分
	
	float playerLength = D3DXVec3Length(&playerDirection);//距離
	D3DXVECTOR3 playerNormVec = playerDirection / playerLength;//正規化
	//D3DXVec3Normalize(&playerDirection, &playerDirection);
	float radian = atan2(playerDirection.z, playerDirection.x);//角度
	
	
	float playerLengthMax = 3;
	float pikminFollowSpeed = 0.1f;
	
	std::random_device rnd;     // 非決定的な乱数生成器を生成
	std::mt19937 mt(rnd());     //  メルセンヌ・ツイスタの32ビット版、引数は初期シード値
	std::uniform_int_distribution<> rand20(-10, 10);        // [-20, 20] 範囲の一様乱数
	
	//playerの当たり判定
	if (playerLength < playerLengthMax)
	{
		m_Position += playerNormVec * pikminFollowSpeed;
	}
	
	D3DXVECTOR3 oldPosition = m_Position;
	
	
	//Line
	Line* line = scene->GetGameObject<Line>(1);
	D3DXVECTOR3 linePosition = line->GetPosition();
	D3DXVECTOR3 lineRotation = line->GetRotation();
	D3DXVECTOR3 lineDirection = m_Position - linePosition;
	float lineLength = D3DXVec3Length(&lineDirection);
	D3DXVECTOR3 lineNormVec = lineDirection / lineLength;

	
	//PlayerとBulletが追従するトリガー
	float lineLengthMax = 6;
	if (Input::GetKeyPress('R') && lineLength < lineLengthMax)
	{
		tracking = true;
	}
	
	/*if (Input::GetKeyTrigger('T'))
	{
		Group_Tracking = true;
	}*/
	//追従してるBulletが追従しなくなるトリガー
	if (Input::GetKeyTrigger('T'))
	{
	
		tracking = false;
	}


	if (tracking == true)//追従してくる動作
	{
		m_Rotation.y = -radian - D3DX_PI / 2;
		//群れ同士の中心
		//m_Position += (AvePosition - m_Position) * 0.01;
		//PlayerについてくるBullet
		trackingSpeed -= playerDirection * m_Speed;

		//ついてきているピクミンを飛ばしたい
		if (Input::GetKeyTrigger(VK_SPACE))
		{
			tracking = false;
			trackingFalse = true;
			m_Rotation = playerRotation;
		
			
		}
		
	}
	
	
	
	if (trackingFalse == true)
	{
		float lineNormVecSpeed = 0.4f;
		m_Position -= lineNormVec * lineNormVecSpeed;
		float trackingFalseLineLengthMax = 0.2f;
		if (lineLength < trackingFalseLineLengthMax)
		{
			trackingFalse = false;
		}
	}

	/*float m_VelocitySpeed = 0.06f;
	m_Velocity.x -= m_Velocity.x * m_VelocitySpeed;
	m_Velocity.z -= m_Velocity.z * m_VelocitySpeed;*/
	//m_Velocity.y -= m_Velocity.y * 2;
	
	//i = 0;

	m_Position += m_Velocity;
	//Bulletの中心にあつまる
	if (groupTracking == true)
	{
		float avePosSpeed = 60;
		trackSpeed += (avePosition - m_Position) * m_Speed * avePosSpeed;
	}

	//その場で止まる
	if (tracking == false)
	{
		
		//m_Position += 0.01f * forward;
		if (m_Position == 0)
		{
			m_Position.x = 0;
			m_Position.y = 0;
			m_Position.z = 0;
		}
		//m_Position.x -= 0.1;
		
		//m_Position.z -= m_Position.z * 0.01;
	}
	
	
	float trackingSpeedMultiplication = 0.05f;
	trackingSpeed.x -= trackingSpeed.x * trackingSpeedMultiplication;
	trackingSpeed.y -= trackingSpeed.y * trackingSpeedMultiplication;
	trackingSpeed.z -= trackingSpeed.z * trackingSpeedMultiplication;

	m_Position += trackingSpeed ;

	//Plyerの4より内側になったら付いてくるスピードが０になる
	float playerLengthStop = 4;
	if (playerLength < playerLengthStop)
	{
		trackingSpeed = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	
	//m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	
	MeshField* meshField = Manager::GetScene()->GetGameObject<MeshField>(1);
	m_Position.y = meshField->GetHeight(m_Position);
	//Enemyのリスト
	std::vector<Enemy*> enemyList = scene->GetGameObjects<Enemy>(1);
	for (Enemy* enemy : enemyList)
	{
		D3DXVECTOR3 enemyPosition = enemy->GetPosition();//enemyのポジション
		D3DXVECTOR3 direction = m_Position - enemyPosition;//enemyとbulletのVECTOR３
		float length = D3DXVec3Length(&direction);//enemyとbulletの距離
		
		
		//if (length < 2)
		//{
		//	////ぶつかったら爆発してデストロイ
		//	//scene->AddGameObject<EXPLOSION>(1)->SetPosition(enemyPosition);
		//	//enemy->SetDestroy();
		//	//SetDestroy();
		//	//return;
		//}
	}

	sum = D3DXVECTOR3(0, 0, 0);//全てのポジションを足したかった？？
	//count = 0;
	//Pikminのリスト
	std::vector<Pikmin*> pikminList = scene->GetGameObjects<Pikmin>(1);
	for (Pikmin* pikmin : pikminList)
	{
		size = pikminList.size();
		pikminPosition = pikmin->GetPosition();
		pikminDirection = m_Position - pikminPosition;//成分
		float pikminLength = D3DXVec3Length(&pikminDirection);//長さ
		
		normVec = pikminDirection / pikminLength;// ノーマライズする

		sum += pikminPosition;

		if (this->GetPosition().x + 2 && this->GetPosition().z + 2)
		{
			if (this != pikmin)//今出てるpikmin(this)とpikminが違ったら 当たり判定
			{


				float pikminLengthMax = 1;
				if (pikminLength < pikminLengthMax)//Pikminどうしの距離判定
				{
					float normVecSpeed = 0.0005f;
					trackPikminSpeed += normVec * normVecSpeed;
					m_Position += trackPikminSpeed;
					//m_Position = oldPosition;

					groupTracking == false;
				}



			}
		}
		
	}
	//Oneのリスト
	std::vector<One*> oneList = scene->GetGameObjects<One>(1);
	for (One* one : oneList)
	{
		D3DXVECTOR3 onePosition = one->GetPosition();
		D3DXVECTOR3 oneScale = one->GetScale();
		D3DXVECTOR3 oneDirection = m_Position - onePosition;

		float oneLength = D3DXVec3Length(&oneDirection);
		D3DXVECTOR3 normVec = oneDirection / oneLength;
		float m_Count = 0;
		//Platformの周囲にきたらとまる
		//Platformについていくようにする
		float oneLengthMax = 5;
		if (oneLength < oneLengthMax)
		{
			notFollow = false;
			trackingFalse = false;
			pikminFollow = true;
		}
		if (pikminFollow == true)
		{
			//m_Position += (onePosition - m_Position) * 0.1;
			float bulletFollowOneLength = 2;
			float oneLengthSpeed = 0.05f;
			if (oneLength < bulletFollowOneLength)
			{
			
				m_Position += normVec * oneLengthSpeed;
			}
			else
			{
				m_Position += (onePosition - m_Position) * oneLengthSpeed;
			}
			pikminFollow = false;
		}
	}
	//Dolphin号のリスト
	{
		Dolphin* dolphin = scene->GetGameObject<Dolphin>(1);
		D3DXVECTOR3 dolphinPosition = dolphin->GetPosition();
		D3DXVECTOR3 dolphinDirection = m_Position - dolphinPosition;
		float dolphinLength = D3DXVec3Length(&dolphinDirection);
		float dolphinLengthMax = 1;

		if (pikminFollow == true && dolphinLength < dolphinLengthMax)
		{

			pikminFollow = false;
			//DolphinStop = true;
			
		}
	}
	
	
	//}
	

	if (pikminFollow == false)
	{
		if (m_Position == 0)
		{
			m_Position.x = 0;
			m_Position.y = 0;
			m_Position.z = 0;
			
		}
	}
	
	//平均のポジション
	avePosition = sum / size;
	if (avePosition == 0)
	{
		tracking = false;
		groupTracking == false;
	}
	
	
	
}

void Pikmin::Draw()
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


/*D3DXVECTOR3 playerForward = player->GetForward();
	D3DXVECTOR3 direction = m_Position - playerPosition;
	float length = D3DXVec3Length(&direction);

	float rad = atan2(playerPosition.x - m_Position.x, playerPosition.y - m_Position.y);
	int x = (int)(cos(rad) * m_Position.x);
	int y = (int)(sin(rad) * m_Position.y);

	if (length > 2)
	{
		m_Position.x = x;
		m_Position.y = y;
	}*/