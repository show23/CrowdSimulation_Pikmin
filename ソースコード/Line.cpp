
#include "Main.h"
#include "Renderer.h"
#include "Model.h"
#include "Manager.h"
#include "Input.h"
#include "Scene.h"
#include "Line.h"
#include "SetRing.h"
#include "Player.h"
#include "MeshField.h"


Model* Line::m_model;//staticのメンバ変数はcppで再度宣言をする


void Line::load()
{

	m_model = new Model();
	m_model->Load("asset\\model\\target.obj");

}
void Line::unload()
{
	m_model->Unload();
	delete m_model;
}
void Line::Init()
{


	
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(0.4f, 0.4f, 0.4f);
	m_Position = D3DXVECTOR3(10.0f, 0.0f, 0.0f);
	platformFollow = false;

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "vertexLightingVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "vertexLightingPS.cso");
}

void Line::Uninit()
{

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void Line::Update()
{
	Scene* scene = Manager::GetScene();
	D3DXVECTOR3 forward = GetForward();
	

	//Player
	Player* player = scene->GetGameObject<Player>(1);
	D3DXVECTOR3 playerPosition = player->GetPosition();
	D3DXVECTOR3 playerRotation = player->GetRotation();
	D3DXVECTOR3 playerDirection = m_Position - playerPosition;
	float playerLength = D3DXVec3Length(&playerDirection);
	D3DXVECTOR3 normVec = playerDirection / playerLength;
	
	int playerLengthMax = 15;
	
	if (playerLength < playerLengthMax)
	{
		float m_PositionSpeed = 0.5f;
		m_Position += forward * m_PositionSpeed;
		m_Rotation = playerRotation;
	}
	else
	{
		float normVecSpeed = 0.1f;
		m_Position -= normVec * normVecSpeed;
	}
	
	
	MeshField* meshField = Manager::GetScene()->GetGameObject<MeshField>(1);
	m_Position.y = meshField->GetHeight(m_Position);

	
}

void Line::Draw()
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
