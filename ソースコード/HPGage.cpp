#include <random>
#include "Main.h"
#include "Manager.h"
#include "Renderer.h"
#include "Camera.h"
#include "Scene.h"
#include "HPGage.h"
#include "Platform.h"
#include "One.h"

void HPGage::Init()
{
	VERTEX_3D vertex[4];

	vertex[0].Position = D3DXVECTOR3(-1.0f, 0.5f, 0.0f);
	vertex[0].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

	vertex[1].Position = D3DXVECTOR3(1.0f, 0.5f, 0.0f);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

	vertex[2].Position = D3DXVECTOR3(-1.0f, -0.5f, 0.0f);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

	vertex[3].Position = D3DXVECTOR3(1.0f, -0.5f, 0.0f);
	vertex[3].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);


	//���_�o�b�t�@����
	D3D11_BUFFER_DESC bd{};

	bd.Usage = D3D11_USAGE_DYNAMIC;//��ŏ����������ł���悤�ɂ������Ă���
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA sd{};
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

	//�e�N�X�`���[�ǂݍ���
	D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
		"asset/texture/HP.jpg",
		NULL,
		NULL,
		&m_Texture,
		NULL);
	assert(m_Texture);

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "unlitTextureVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "unlitTexturePS.cso");

	m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	m_Count = 0.0f;

}

void HPGage::Uninit()
{
	m_VertexBuffer->Release();
	m_Texture->Release();
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void HPGage::Update()
{
	m_Count++;
}

void HPGage::Draw()
{
	
	//Platform* platform = scene->GetGameObject<Platform>(1);
	
	float xMax = 1.8f;
	int m_CountDivision = 50;
	x = xMax - (m_Count / m_CountDivision);

	

	Scene* scene = Manager::GetScene();
	std::vector<Platform*> platformList = scene->GetGameObjects<Platform>(1);
	for (Platform* platform : platformList)
	{
		
		//float length = D3DXVec3Length(&direction);//enemy��bullet�̋���
		D3DXVECTOR3 platformPosition = platform->GetPosition();
		D3DXVECTOR3 direction = m_Position - platformPosition;//platform��bullet��VECTOR�R
		float length = D3DXVec3Length(&direction);//platform��bullet�̋���
		
		if (x < -xMax)
		{
			SetDestroy();

			m_HPGageDestroy = true;
			platform->SetDestroy();
			if (m_HPGageDestroy == true)
			{
				scene->AddGameObject<One>(1)->SetPosition(platformPosition);
				m_HPGageDestroy = false;
			}
			
		}
		
		//if (flowerGeneretor == true)
		//{
		//	scene->AddGameObject<Platform>(1)->SetPosition(D3DXVECTOR3(10, 0.0f, -1.0f));
		//	flowerCount++;
		//	if (flowerCount > 10)
		//	{
		//		
		//		/*flowerGeneretor = false;
		//		flowerCount = 0;*/
		//	}
		//}

		
	}
	
	//���_�f�[�^��������
	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0,
		D3D11_MAP_WRITE_DISCARD, 0, &msr);
	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	vertex[0].Position = D3DXVECTOR3(-1.8f, 0.4f, 0.0f);
	vertex[0].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);
	//(1.0f, 1.0f, 1.0f, 1.0f);
	//(0.5f, 0.5f, 0.5f, 0.5f);
	vertex[1].Position = D3DXVECTOR3(x, 0.4f, 0.0f);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

	vertex[2].Position = D3DXVECTOR3(-1.8f, -0.4f, 0.0f);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

	vertex[3].Position = D3DXVECTOR3(x, -0.4f, 0.0f);
	vertex[3].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);


	Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);

	//���̓��C�A�E�g
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//�V�F�[�_�[�ݒ�
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//�r���{�[�h����
	//�J�����̃r���[�}�g���b�N�X�擾
	
	Camera* camera = scene->GetGameObject<Camera>(0);
	D3DXMATRIX view = camera->GetViewMatrix();

	//�r���[�̋t�s��
	D3DXMATRIX invView;
	D3DXMatrixInverse(&invView, NULL, &view);//�t�s��
	invView._41 = 0.0f;
	invView._42 = 0.0f;
	invView._43 = 0.0f;

	//���[���h�}�g���b�N�X�ݒ�
	D3DXMATRIX world, scale, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	//D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.x, m_Rotation.y, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);

	world = scale * invView * trans;
	Renderer::SetWorldMatrix(&world);

	//���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	//�}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Renderer::SetMaterial(material);

	//�e�N�X�`���[�ݒ�
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	//�v���~�e�B�u�g�|���W�ݒ�
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//�|���S���`��
	Renderer::GetDeviceContext()->Draw(4, 0);
}