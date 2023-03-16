#include "Main.h"
#include "Renderer.h"
#include "Input.h"
#include "Gameobject.h"
#include "Camera.h"
#include "Model.h"
#include "Scene.h"
#include "Manager.h"
#include "Player.h"
#include "Pikmin.h"
#include "Audio.h"
#include "Shadow.h"
#include "Line.h"
#include "Platform.h"
#include "SetRing.h"
#include "MeshField.h"
#include "Game.h"

void Player::Init()
{
	

	m_model = new Model();
	m_model->Load("asset\\model\\orima-.obj");


	m_Position = D3DXVECTOR3(0.0f, 10.0f, -10.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(0.3f, 0.3f, 0.3f);

	
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "vertexLightingVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "vertexLightingPS.cso");

	Scene* scene = Manager::GetScene();
	m_ShotSE = scene->AddGameObject<Audio>(2);
	m_ShotSE->Load("asset\\audio\\wan.wav");

	m_Shadow = scene->AddGameObject<Shadow>(1);
	m_Shadow->SetPosition(m_Position);
	//m_Shadow->SetScale(D3DXVECTOR3(2.0f, 1.0f, 2.0f));

	m_Velocity = D3DXVECTOR3(0, 0, 0);
	m_TargetRotation = D3DXVECTOR3(0, 0, 0);
	playerLength = D3DXVECTOR3(0, 0, 0);
	
	m_Count = 0;
	lineTrigger = false;
	MinLength = 1000.0f;
	
}

void Player::Uninit()
{
	m_model->Unload();
	delete m_model;

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void Player::Update()
{
	//m_Rotation = D3DXVECTOR3(2.0f, 0.0f, 0.0f);
	
	Scene* scene = Manager::GetScene();
	
	float m_RotationSpeed = 0.1f;
	/*if (Input::GetKeyPress('Q'))
	{
		m_Rotation.y -= m_RotationSpeed;
	}
	if (Input::GetKeyPress('E'))
	{
		m_Rotation.y+= m_RotationSpeed;
	}
	*/

	D3DXVECTOR3 forward = GetForward();
	
	float m_TargetRotationSpeed = 0.5f;
	if (Input::GetKeyPress('A'))
	{
		m_TargetRotation.y = -D3DX_PI * m_TargetRotationSpeed;
	}
	if (Input::GetKeyPress('D'))
	{
		m_TargetRotation.y = D3DX_PI* m_TargetRotationSpeed;
	}
	if (Input::GetKeyPress('W'))
	{
		m_TargetRotation.y = 0;
	}
	if (Input::GetKeyPress('S'))
	{
		m_TargetRotation.y = -D3DX_PI;
	}
	if (Input::GetKeyTrigger('J'))
	{
		//m_Position.y -= D3DX_PI;
	}



	MeshField* meshField = Manager::GetScene()->GetGameObject<MeshField>(1);
	m_Position.y = meshField->GetHeight(m_Position);


	D3DXVECTOR3 dist = m_TargetRotation - m_Rotation;
	
	float distSpeed = 2.0f;
	if (dist.y > D3DX_PI)
		dist.y -= D3DX_PI * distSpeed;
	else if(dist.y < -D3DX_PI)
		dist.y += D3DX_PI * distSpeed;

	if (m_Rotation.y > D3DX_PI)
		m_Rotation.y -= D3DX_PI * distSpeed;
	else if (m_Rotation.y < -D3DX_PI)
		m_Rotation.y += D3DX_PI * distSpeed;

	float distMultiplication = 0.1f;
	m_Rotation += dist * distMultiplication;

	if (Input::GetKeyPress('A') || Input::GetKeyPress('D') || Input::GetKeyPress('W') || Input::GetKeyPress('S'))
	{
		float forwardMultiplication = 0.025f;
		m_Velocity += forward * forwardMultiplication;
	}
	float m_VelocitySpeed = 0.1f;
	//m_Velocity.y -= 0.01f;
	m_Velocity.x -= m_Velocity.x * m_VelocitySpeed;
	//m_Velocity.y -= m_Velocity.y * 0.01;
	m_Velocity.z -= m_Velocity.z * m_VelocitySpeed;


	D3DXVECTOR3 oldPosition = m_Position;
	m_Position += m_Velocity;

	/*if (m_Position.y < 0.0f)
	{
		m_Position.y = 0.0f;
		m_Velocity.y = 0.0f;
	}*/
	float groundHeight = 0.0f;

	
	//Platformリスト
	std::vector<Platform*> platformList = scene->GetGameObjects<Platform>(1);
	for (Platform* platform : platformList)
	{
		D3DXVECTOR3 platformPosition = platform->GetPosition();
		D3DXVECTOR3 platformScale = platform->GetScale();
		D3DXVECTOR3 direction = m_Position - platformPosition;
		direction.y = 0.0f;
		float length = D3DXVec3Length(&direction);

		if (length < platformScale.x)
		{
			if (m_Position.y < platformPosition.y + platformScale.y - 0.5f)
			{
				m_Position.x = oldPosition.x;
				m_Position.z = oldPosition.z;

			}
			else
			{
				groundHeight = platformPosition.y + platformScale.y;
			}
			break;
			
		}
	}

	//Line
	Line* line = scene->GetGameObject<Line>(1);
	D3DXVECTOR3 linePosition = line->GetPosition();
	D3DXVECTOR3 lineRotation = line->GetRotation();
	D3DXVECTOR3 lineDirection = m_Position - linePosition;
	float lineLength = D3DXVec3Length(&lineDirection);
	D3DXVECTOR3 line_norm_vec = lineDirection / lineLength;

	std::vector<Pikmin*> pikminList = scene->GetGameObjects<Pikmin>(1);
	for (Pikmin* pikmin : pikminList)
	{
		D3DXVECTOR3 pikminPosition = pikmin->GetPosition();
		D3DXVECTOR3 pikminDirection = pikminPosition - m_Position;
		float pikminLength = D3DXVec3Length(&pikminDirection);

		if (MinLength > pikminLength)
		{
			MinLength = pikminLength;
		}
	}
	/*D3DXVECTOR3 shadowPos = m_Position;
	shadowPos.y = 0.0f;
	m_Shadow->SetPosition(shadowPos);*/
}

void Player::Draw()
{
	//視錐台カリング
	Scene* scene = Manager::GetScene();
	Camera* camera = scene->GetGameObject<Camera>(0);

	if (!camera->CheckView(m_Position))
		return;

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