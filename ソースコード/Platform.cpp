
#include "Main.h"
#include "Renderer.h"
#include "Model.h"
#include "Manager.h"
#include "Input.h"
#include "Platform.h"
#include "Scene.h"
#include "Pikmin.h"
#include "Dolphin.h"
#include "HPGage.h"
#include "One.h"

Model* Platform::m_model;//staticのメンバ変数はcppで再度宣言をする


void Platform::load()
{

	m_model = new Model();
	m_model->Load("asset\\model\\floawer.obj");

}
void Platform::unload()
{
	m_model->Unload();
	delete m_model;
}
void Platform::Init()
{


	m_Position = D3DXVECTOR3(10.0f, 0.0f, 10.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(0.6f, 0.6f, 0.6f);
	platformFollow = false;

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "vertexLightingVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "vertexLightingPS.cso");
}

void Platform::Uninit()
{

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void Platform::Update()
{
	Scene* scene = Manager::GetScene();
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	float Hight = m_Position.y + 8.0f;
	//HPバーの表示位置
	D3DXVECTOR3 HP = D3DXVECTOR3(m_Position.x, Hight, m_Position.z);

	//bulletのList
	std::vector<Pikmin*> pikminList = scene->GetGameObjects<Pikmin>(1);
	for (Pikmin* pikmin : pikminList)
	{
		D3DXVECTOR3 pikminPosition = pikmin->GetPosition();
		D3DXVECTOR3 pikminDirection = m_Position - pikminPosition;
		float pikminLength = D3DXVec3Length(&pikminDirection);
		float pikminLengthMax = 5;

		if (pikminLength < pikminLengthMax)
		{
			platformFollow = true;
			if (once == false)
			{
				
				scene->AddGameObject<HPGage>(1)->SetPosition(HP);
				once = true;
			}
		}
	}
	//m_Count;

	

	
}

void Platform::Draw()
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
