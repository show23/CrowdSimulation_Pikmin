

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
#include "BluePikmin.h"
#include "Line.h"
#include "Explosion.h"
#include "Enemy.h"
#include "One.h"
#include "Platform.h"
#include "MeshField.h"

#include "dolphin.h"


Model* BluePikmin::m_model;//static�̃����o�ϐ���cpp�ōēx�錾������
void BluePikmin::load()
{
	//���f���̃Z�b�g
	m_model = new Model();
	m_model->Load("asset\\model\\BluePikumin.obj");
}

void BluePikmin::unload()
{
	//���f���̃A�����[�h
	m_model->Unload();
	delete m_model;
}


void BluePikmin::Init()
{

	m_Position = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(0.2f, 0.2f, 0.2f);
	m_Gravity = D3DXVECTOR3(0.0f, 0.005f, 0.0f);
	trackingSpeed = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//bullet��Player�ɂ��Ă���X�s�[�h
	trackSpeed = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	trackBulletSpeed = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//Bullet�ǂ����̓����蔻��Ɏg���Ă��锽���̒l�Ȃǂ����Ă�
	normVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//Bullet���m�̓����蔻��̔����i�m�[�}���C�Y���Ă��遖�f�[�^�̃X�P�[���������₷�����̂ɐ����邱�Ɓj
	m_Velocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Speed = 0.001f;//�Ǐ]���鑬�x�ɂ����Ă�l
	upForce = 0.2f;//�W�����v���鑬�x�i���͎g���Ă��Ȃ��j

	/*�x�N�g���v�Z�i���͎g���ĂȂ��j
	dx = 0.0f;
	dy = 0.0f;
	l = 0.0f;
	x1 = 0.0f;
	y1 = 0.0f;*/


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

void BluePikmin::Uninit()
{


	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void BluePikmin::Update()
{
	D3DXVECTOR3 forward = GetForward();
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>(1);
	D3DXVECTOR3 playerPosition = player->GetPosition();
	D3DXVECTOR3 playerRotation = player->GetRotation();
	D3DXVECTOR3 playerDirection = m_Position - playerPosition;

	float playerLength = D3DXVec3Length(&playerDirection);
	float bulletFollowSpeed = 0.1f;
	D3DXVECTOR3 playerNormVec = playerDirection / playerLength;
	float radian = atan2(playerDirection.z, playerDirection.x);
	float playerLengthMax = 3;

	if (playerLength < playerLengthMax)
	{
		m_Position += playerNormVec * bulletFollowSpeed;
	}

	D3DXVECTOR3 oldPosition = m_Position;


	
	
	//Player��Bullet���Ǐ]����g���K�[
	//Line
	Line* line = scene->GetGameObject<Line>(1);
	D3DXVECTOR3 linePosition = line->GetPosition();
	D3DXVECTOR3 lineRotation = line->GetRotation();
	D3DXVECTOR3 lineDirection = m_Position - linePosition;
	float lineLength = D3DXVec3Length(&lineDirection);
	D3DXVECTOR3 lineNormVec = lineDirection / lineLength;
	float lineLengthMax = 6;

	//Player��Bullet���Ǐ]����g���K�[
	if (Input::GetKeyPress('R') && lineLength < lineLengthMax)
	{
		tracking = true;
	}
	if (Input::GetKeyTrigger('Z'))
	{
		groupTracking = true;
	}
	//�Ǐ]���Ă�Bullet���Ǐ]���Ȃ��Ȃ�g���K�[
	if (Input::GetKeyTrigger('T'))
	{

		tracking = false;
		groupTracking == false;
	}


	if (tracking == true)//�Ǐ]���Ă��铮��
	{
		m_Rotation.y = -radian + D3DX_PI / 2;
		//�Q�ꓯ�m�̒��S
		//m_Position += (AvePosition - m_Position) * 0.01;
		//Player�ɂ��Ă���Bullet
		trackingSpeed += (playerPosition - m_Position) * m_Speed;

		//���Ă��Ă���s�N�~�����΂�����
		if (Input::GetKeyTrigger(VK_SPACE))
		{
			tracking = false;
			trackingFalse = true;
			m_Rotation = playerRotation;
		}

	}

	float lineNormVecSpeed = 0.4f;//lineNormVec�Ɋ|����l
	float m_VelocitySpeed = 0.06f;//m_Velocity�Ɋ|����l
	if (trackingFalse == true)
	{
		m_Position -= lineNormVec * lineNormVecSpeed;
	}
	
	m_Velocity.x -= m_Velocity.x * m_VelocitySpeed;
	m_Velocity.z -= m_Velocity.z * m_VelocitySpeed;
	//m_Velocity.y -= m_Velocity.y * 2;

	

	m_Position += m_Velocity;
	//Bullet�̒��S�ɂ��܂�
	if (groupTracking == true)
	{
		if (tracking == true)
		{
			float avePosSpeed = 60;
			m_Position += (avePosition - m_Position) * m_Speed * avePosSpeed;
		}
		
	}

	//���̏�Ŏ~�܂�
	if (tracking == false)
	{

		
		if (m_Position == 0)
		{
			m_Position.x = 0;
			m_Position.y = 0;
			m_Position.z = 0;
		}
		
	}

	//���Ă���X�s�[�h
	float trackingSpeedMultiplication = 0.05f;
	trackingSpeed.x -= trackingSpeed.x * trackingSpeedMultiplication;
	trackingSpeed.y -= trackingSpeed.y * trackingSpeedMultiplication;
	trackingSpeed.z -= trackingSpeed.z * trackingSpeedMultiplication;

	m_Position += trackingSpeed;

	//Plyer��4�������ɂȂ�����t���Ă���X�s�[�h���O�ɂȂ�
	float playerLengthStop = 4;
	if (playerLength < playerLengthStop)
	{
		trackingSpeed = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	
	sumX += trackingSpeed;
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
		//	//�Ԃ������甚�����ăf�X�g���C
		//	scene->AddGameObject<EXPLOSION>(1)->SetPosition(enemyPosition);
		//	enemy->SetDestroy();
		//	SetDestroy();
		//	return;
		//}
	}

	sum = D3DXVECTOR3(0, 0, 0);//�S�Ẵ|�W�V�����𑫂����������H�H
	//count = 0;

	//BlueBullet�̃��X�g
	std::vector<BluePikmin*> bluePikminList = scene->GetGameObjects<BluePikmin>(1);
	for (BluePikmin* bluePikmin : bluePikminList)
	{
		size = bluePikminList.size();
		bluePikminPosition = bluePikmin->GetPosition();
		bluePikminDirection = m_Position - bluePikminPosition;



		float bluePikminLength = D3DXVec3Length(&bluePikminDirection);
		// �m�[�}���C�Y����
		normVec = bluePikminDirection / bluePikminLength;


		sum += bluePikminPosition;

		/*if (Maxlength > playerLength)
		{
			Maxlength = playerLength;
			pikmin = this;
		}*/

		if (this == bluePikmin);//���o�Ă�o���b�g(this)��bullet��������������
		{
			//sum = pikminPosition.x;
		}

		if (this != bluePikmin)//���o�Ă�o���b�g(this)��bullet��������� ���̓����蔻��
		{

			float bluePikminLengthMax = 1;
			if (bluePikminLength && bluePikminLength < bluePikminLengthMax)
			{
				float normVecSpeed = 0.0005f;
				trackBulletSpeed += normVec * normVecSpeed;
				m_Position += trackBulletSpeed;
				//m_Position = oldPosition;

				groupTracking == false;
			}

		}
		
	}
	//Bullet�̃��X�g
	std::vector<Pikmin*> pikminList = scene->GetGameObjects<Pikmin>(1);
	for (Pikmin* pikmin : pikminList)
	{
		D3DXVECTOR3 pikminPosition = pikmin->GetPosition();
		D3DXVECTOR3 pikminScale = pikmin->GetScale();
		D3DXVECTOR3 pikminDirection = m_Position - pikminPosition;
		pikminDirection.y = 0.0f;
		float pikminlength = D3DXVec3Length(&pikminDirection);
		D3DXVECTOR3 normVec = pikminDirection / pikminlength;
		float pikminLengthMax = 1;

		if (pikminlength && pikminlength < pikminLengthMax)
		{
			float trackBulletMultiplication = 0.0008f;
			trackBulletSpeed += normVec * trackBulletMultiplication;
			m_Position += trackBulletSpeed * trackBulletMultiplication;
			//m_Position = oldPosition;

			groupTracking == false;
		}
	}

	//One�̃��X�g
	std::vector<One*> oneList = scene->GetGameObjects<One>(1);
	for (One* one : oneList)
	{
		D3DXVECTOR3 onePosition = one->GetPosition();
		D3DXVECTOR3 oneScale = one->GetScale();
		D3DXVECTOR3 onedDirection = m_Position - onePosition;

		float oneLength = D3DXVec3Length(&onedDirection);
		D3DXVECTOR3 normVec = onedDirection / oneLength;
		float m_Count = 0;
		//Platform�̎��͂ɂ�����Ƃ܂�
		//Platform�ɂ��Ă����悤�ɂ���
		float oneLengthMax = 5;
		if (oneLength < oneLengthMax)
		{
			notFollow = false;
			trackingFalse = false;
			bulletFollow = true;
		}
		if (bulletFollow == true)
		{
			//m_Position += (onePosition - m_Position) * 0.1;
			float bulletFollowOneLengthMax = 2;
			float oneLengthNormVecSpeed = 0.05;
			if (oneLength < bulletFollowOneLengthMax)
			{
				m_Position += normVec * oneLengthNormVecSpeed;
			}
			else
			{
				m_Position += (onePosition - m_Position) * oneLengthNormVecSpeed;
			}
			bulletFollow = false;
		}
	}
	
	//Dolphin���̃��X�g
	std::vector<Dolphin*> dolphinList = scene->GetGameObjects<Dolphin>(1);
	for (Dolphin* dolphin : dolphinList)
	{
		D3DXVECTOR3 dolphinPosition = dolphin->GetPosition();
		D3DXVECTOR3 dolphinDirection = m_Position - dolphinPosition;
		float dolphinLength = D3DXVec3Length(&dolphinDirection);

		float dolphineLengthMax = 1;
		if (bulletFollow == true && dolphineLengthMax < dolphineLengthMax)
		{

			bulletFollow = false;
			
		}

	}

	if (bulletFollow == false)
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

	count;


}

void BluePikmin::Draw()
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