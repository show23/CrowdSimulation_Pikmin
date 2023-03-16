
#include "Main.h"
#include "Renderer.h"
#include "Model.h"
#include "Manager.h"
#include "Platform.h"
#include "Scene.h"
#include "Dolphin.h"
#include "Pikmin.h"

//�����ɕ����^��

Model* Dolphin::m_model;//static�̃����o�ϐ���cpp�ōēx�錾������


void Dolphin::load()
{

	m_model = new Model();
	m_model->Load("asset\\model\\dolphin.obj");

}
void Dolphin::unload()
{
	m_model->Unload();
	delete m_model;
}
void Dolphin::Init()
{


	//m_Position = D3DXVECTOR3(15.0f, 0.0f, 10.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);


	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "vertexLightingVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "vertexLightingPS.cso");
}

void Dolphin::Uninit()
{

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void Dolphin::Update()
{
	Scene* scene = Manager::GetScene();
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	////bullet��List
	//std::vector<Bullet*> bulletList = scene->GetGameObjects<Bullet>(1);
	//for (Bullet* bullet : bulletList)
	//{
	//	D3DXVECTOR3 bulletPosition = bullet->GetPosition();
	//	D3DXVECTOR3 bulletdirection = m_Position - bulletPosition;
	//	float bulletlength = D3DXVec3Length(&bulletdirection);

	//	if (bulletlength < 4)
	//	{
	//		m_Position.x -= 0.1;
	//	}
	//}

}

void Dolphin::Draw()
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

