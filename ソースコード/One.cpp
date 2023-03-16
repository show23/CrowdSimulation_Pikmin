#include <random>
#include "Main.h"
#include "Renderer.h"
#include "Model.h"
#include "Manager.h"
#include "Input.h"
#include "MeshField.h"
#include "Platform.h"
#include "Scene.h"
#include "Pikmin.h"
#include "Dolphin.h"
#include "HPGage.h"
#include "One.h"

Model* One::m_model;//static�̃����o�ϐ���cpp�ōēx�錾������


void One::load()
{

	m_model = new Model();
	m_model->Load("asset\\model\\1.obj");

}
void One::unload()
{
	m_model->Unload();
	delete m_model;
}
void One::Init()
{


	m_Position = D3DXVECTOR3(10.0f, 0.0f, 10.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(0.6f, 0.6f, 0.6f);
	platformFollow = false;

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "vertexLightingVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "vertexLightingPS.cso");
}

void One::Uninit()
{

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void One::Update()
{
	Scene* scene = Manager::GetScene();
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//���b�V���t�B�[���h�K�p
	MeshField* meshField = Manager::GetScene()->GetGameObject<MeshField>(1);
	m_Position.y = meshField->GetHeight(m_Position);

	//Pikmin��List
	std::vector<Pikmin*> bulletList = scene->GetGameObjects<Pikmin>(1);
	for (Pikmin* pikmin : bulletList)
	{
		D3DXVECTOR3 bulletPosition = pikmin->GetPosition();
		D3DXVECTOR3 bulletDirection = m_Position - bulletPosition;
		float bulletLength = D3DXVec3Length(&bulletDirection);
		float bulletLengthMax = 5;
		if (bulletLength < bulletLengthMax)
		{
			platformFollow = true;
		}
	}

	std::vector<Dolphin*> dolphinList = scene->GetGameObjects<Dolphin>(1);
	for (Dolphin* dolphin : dolphinList)
	{
		D3DXVECTOR3 dolphinPosition = dolphin->GetPosition();
		D3DXVECTOR3 dolphinDirection = m_Position - dolphinPosition;
		float dolphinLength = D3DXVec3Length(&dolphinDirection);
		std::random_device rnd;     // �񌈒�I�ȗ���������𐶐�
		std::mt19937 mt(rnd());     //  �����Z���k�E�c�C�X�^��32�r�b�g�ŁA�����͏����V�[�h�l
		std::uniform_int_distribution<> rand20(-10, 10);        // [-20, 20] �͈͂̈�l����
		

		if (platformFollow == true)
		{

			count++;
			float countMin = 60;
			if (count > countMin)
			{
				float m_PositionSpeed = 0.005f;
				m_Position -= dolphinDirection * m_PositionSpeed;
			}

		}
		float dolphineLengthMax = 3;
		if (dolphinLength < dolphineLengthMax && platformFollow == true)
		{
			SetDestroy();
			
			
			if (once == false)
			{
				Pikmin* bullet = scene->AddGameObject<Pikmin>(1);
				bullet = scene->AddGameObject<Pikmin>(1);
				bullet->SetPosition(D3DXVECTOR3(rand20(mt), 1, rand20(mt)));

				scene->AddGameObject<Platform>(1)->SetPosition(D3DXVECTOR3(rand20(mt), 1, rand20(mt)));
				
				once = true;
			}
			count = 0;
		}

		
	}


}

void One::Draw()
{
	//���̓��C�A�E�g
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//�V�F�[�_�[�ݒ�
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//���[���h�}�g���b�N�X�ݒ�
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.x, m_Rotation.y, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);

	world = scale * rot * trans;
	Renderer::SetWorldMatrix(&world);

	m_model->Draw();

}

