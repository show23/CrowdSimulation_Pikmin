
#include "Main.h"
#include "Renderer.h"
#include "Model.h"
#include "Manager.h"
#include "Input.h"
#include "Game.h"
#include "Gameobject.h"
#include "Scene.h"
#include "Line.h"
#include "SetRing.h"
#include "Player.h"
#include "MeshField.h"


Model* SetRing::m_model;//staticのメンバ変数はcppで再度宣言をする


void SetRing::load()
{

	m_model = new Model();
	m_model->Load("asset\\model\\shugou.obj");

}
void SetRing::unload()
{
	m_model->Unload();
	delete m_model;
}
void SetRing::Init()
{


	float xScale = 0.4f;
	float yScale = 0.2f;
	float zScale = 0.4f;
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(xScale, yScale, zScale);
	m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	platformFollow = false;

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "vertexLightingVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "vertexLightingPS.cso");
}

void SetRing::Uninit()
{

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void SetRing::Update()
{
	
	Scene* scene = Manager::GetScene();
	
	//Player
	Line* line = scene->GetGameObject<Line>(1);
	D3DXVECTOR3 linePosition = line->GetPosition();
	D3DXVECTOR3 lineRotation = line->GetRotation();
	D3DXVECTOR3 lineDirection = m_Position - linePosition;
	float lineLength = D3DXVec3Length(&lineDirection);
	D3DXVECTOR3 normVec = lineDirection / lineLength;

	

	m_Position = linePosition;


}

void SetRing::Draw()
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


	if (Input::GetKeyPress('R') )
	{
		m_model->Draw();
	}
	

}

