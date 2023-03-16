
#include "Main.h"
#include "Renderer.h"
#include "Model.h"
#include "Manager.h"
#include "Scene.h"
#include "Sky.h"




Model* Sky::m_model;//staticのメンバ変数はcppで再度宣言をする


void Sky::load()
{

	m_model = new Model();
	m_model->Load("asset\\model\\Sky_01.obj");

}
void Sky::unload()
{
	m_model->Unload();
	delete m_model;
}
void Sky::Init()
{


	//m_Position = D3DXVECTOR3(15.0f, 0.0f, 10.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(100.0f, 100.0f, 100.0f);


	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "vertexLightingVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "vertexLightingPS.cso");
}

void Sky::Uninit()
{

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void Sky::Update()
{
	Scene* scene = Manager::GetScene();
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);


}

void Sky::Draw()
{
	//入力レイアウト
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//ワールドマトリックス設定
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.x, m_Rotation.y, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);

	world = scale * rot * trans;
	Renderer::SetWorldMatrix(&world);

	m_model->Draw();

}

