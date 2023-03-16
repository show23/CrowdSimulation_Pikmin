
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

Model* Pikmin::m_model;//static�̃����o�ϐ���cpp�ōēx�錾������
void Pikmin::load()
{
	//���f���̃Z�b�g
	m_model = new Model();
	m_model->Load("asset\\model\\Pikumin1018.obj");
}

void Pikmin::unload()
{
	//���f���̃A�����[�h
	m_model->Unload();
	delete m_model;
}


void Pikmin::Init()
{
	
	m_Position = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(0.2f, 0.2f, 0.2f);
	m_Gravity = D3DXVECTOR3(0.0f, 0.005f, 0.0f);
	trackingSpeed = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//bullet��Player�ɂ��Ă���X�s�[�h
	trackSpeed = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//���S�ɏW�܂鑬�x
	trackPikminSpeed = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//Bullet�ǂ����̓����蔻��Ɏg���Ă��锽���̒l�Ȃǂ����Ă�
	normVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//Bullet���m�̓����蔻��̔����i�m�[�}���C�Y���Ă��遖�f�[�^�̃X�P�[���������₷�����̂ɐ����邱�Ɓj
	m_Velocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Speed = 0.001f;//�Ǐ]���鑬�x�ɂ����Ă�l
	upForce = 0.2f;//�W�����v���鑬�x�i���͎g���Ă��Ȃ��j

	
	
	maxLength = 10000.0f;//bullet��Player�̍ŒZ�����̂��̂�T�����߂̍X�V����l
	
	vecSpeedx = 0.0f;
	vecSpeedy = 0.0f;
	m_TargetRotation = D3DXVECTOR3(0, 0, 0);
	
	tracking = false;//Bullet��Player�ɂ��Ă��邩�ǂ����̐^�U
	trackingFalse = false;//Tracking���U�ɂȂ��Ă�Ƃ��݂̂̐^�U
	groupTracking = false;//�s�N�~�����m���W�܂����Ƃ��̐^�U
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
	D3DXVECTOR3 playerDirection = m_Position - playerPosition;//����
	
	float playerLength = D3DXVec3Length(&playerDirection);//����
	D3DXVECTOR3 playerNormVec = playerDirection / playerLength;//���K��
	//D3DXVec3Normalize(&playerDirection, &playerDirection);
	float radian = atan2(playerDirection.z, playerDirection.x);//�p�x
	
	
	float playerLengthMax = 3;
	float pikminFollowSpeed = 0.1f;
	
	std::random_device rnd;     // �񌈒�I�ȗ���������𐶐�
	std::mt19937 mt(rnd());     //  �����Z���k�E�c�C�X�^��32�r�b�g�ŁA�����͏����V�[�h�l
	std::uniform_int_distribution<> rand20(-10, 10);        // [-20, 20] �͈͂̈�l����
	
	//player�̓����蔻��
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

	
	//Player��Bullet���Ǐ]����g���K�[
	float lineLengthMax = 6;
	if (Input::GetKeyPress('R') && lineLength < lineLengthMax)
	{
		tracking = true;
	}
	
	/*if (Input::GetKeyTrigger('T'))
	{
		Group_Tracking = true;
	}*/
	//�Ǐ]���Ă�Bullet���Ǐ]���Ȃ��Ȃ�g���K�[
	if (Input::GetKeyTrigger('T'))
	{
	
		tracking = false;
	}


	if (tracking == true)//�Ǐ]���Ă��铮��
	{
		m_Rotation.y = -radian - D3DX_PI / 2;
		//�Q�ꓯ�m�̒��S
		//m_Position += (AvePosition - m_Position) * 0.01;
		//Player�ɂ��Ă���Bullet
		trackingSpeed -= playerDirection * m_Speed;

		//���Ă��Ă���s�N�~�����΂�����
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
	//Bullet�̒��S�ɂ��܂�
	if (groupTracking == true)
	{
		float avePosSpeed = 60;
		trackSpeed += (avePosition - m_Position) * m_Speed * avePosSpeed;
	}

	//���̏�Ŏ~�܂�
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

	//Plyer��4�������ɂȂ�����t���Ă���X�s�[�h���O�ɂȂ�
	float playerLengthStop = 4;
	if (playerLength < playerLengthStop)
	{
		trackingSpeed = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	
	//m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	
	MeshField* meshField = Manager::GetScene()->GetGameObject<MeshField>(1);
	m_Position.y = meshField->GetHeight(m_Position);
	//Enemy�̃��X�g
	std::vector<Enemy*> enemyList = scene->GetGameObjects<Enemy>(1);
	for (Enemy* enemy : enemyList)
	{
		D3DXVECTOR3 enemyPosition = enemy->GetPosition();//enemy�̃|�W�V����
		D3DXVECTOR3 direction = m_Position - enemyPosition;//enemy��bullet��VECTOR�R
		float length = D3DXVec3Length(&direction);//enemy��bullet�̋���
		
		
		//if (length < 2)
		//{
		//	////�Ԃ������甚�����ăf�X�g���C
		//	//scene->AddGameObject<EXPLOSION>(1)->SetPosition(enemyPosition);
		//	//enemy->SetDestroy();
		//	//SetDestroy();
		//	//return;
		//}
	}

	sum = D3DXVECTOR3(0, 0, 0);//�S�Ẵ|�W�V�����𑫂����������H�H
	//count = 0;
	//Pikmin�̃��X�g
	std::vector<Pikmin*> pikminList = scene->GetGameObjects<Pikmin>(1);
	for (Pikmin* pikmin : pikminList)
	{
		size = pikminList.size();
		pikminPosition = pikmin->GetPosition();
		pikminDirection = m_Position - pikminPosition;//����
		float pikminLength = D3DXVec3Length(&pikminDirection);//����
		
		normVec = pikminDirection / pikminLength;// �m�[�}���C�Y����

		sum += pikminPosition;

		if (this->GetPosition().x + 2 && this->GetPosition().z + 2)
		{
			if (this != pikmin)//���o�Ă�pikmin(this)��pikmin��������� �����蔻��
			{


				float pikminLengthMax = 1;
				if (pikminLength < pikminLengthMax)//Pikmin�ǂ����̋�������
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
	//One�̃��X�g
	std::vector<One*> oneList = scene->GetGameObjects<One>(1);
	for (One* one : oneList)
	{
		D3DXVECTOR3 onePosition = one->GetPosition();
		D3DXVECTOR3 oneScale = one->GetScale();
		D3DXVECTOR3 oneDirection = m_Position - onePosition;

		float oneLength = D3DXVec3Length(&oneDirection);
		D3DXVECTOR3 normVec = oneDirection / oneLength;
		float m_Count = 0;
		//Platform�̎��͂ɂ�����Ƃ܂�
		//Platform�ɂ��Ă����悤�ɂ���
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
	//Dolphin���̃��X�g
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
	
	//���ς̃|�W�V����
	avePosition = sum / size;
	if (avePosition == 0)
	{
		tracking = false;
		groupTracking == false;
	}
	
	
	
}

void Pikmin::Draw()
{
	
	//���̓��C�A�E�g
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//�V�F�[�_�[�ݒ�
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//���[���h�}�g���b�N�X�ݒ�
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