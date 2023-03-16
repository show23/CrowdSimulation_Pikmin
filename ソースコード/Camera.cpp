#include "Main.h"
#include "Manager.h"
#include "Renderer.h"
#include "Scene.h"
#include "Input.h"
#include "Polygon2D.h"
#include "Player.h"
#include "Camera.h"



void Camera::Init()
{
	m_Position = D3DXVECTOR3(0.0f, 10.0f, -15.0f);
	m_Target = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	
}

void Camera::Uninit()
{

}

void Camera::Update()
{
    Scene* scene = Manager::GetScene();
    Player* player = scene->GetGameObject<Player>(1);
    D3DXVECTOR3 playerPosition = player->GetPosition();
    D3DXVECTOR3 playerForward = player->GetForward();
    
    //トップビュー
    m_Target = playerPosition;
    m_Position = playerPosition + D3DXVECTOR3(0.0f, 12.0f, -20.0f);
   /* if (Input::GetKeyPress('S'))
    {
        m_Position.y -=  0.5;
    }*/
   
    ////サードパーソンビュー
    //m_Target = playerPosition;
    //m_Position = playerPosition - playerForward  *  10.0f + D3DXVECTOR3(0.0f, 5.0f,0.0f);

    ////ファーストパーソンビュー
    //m_Target = playerPosition + playerForward;
    //m_Position = playerPosition ;
}

void Camera::Draw()
{
    float upY = 1.0f;
    D3DXVECTOR3 up = D3DXVECTOR3(0.0f, upY, 0.0f);

    D3DXMatrixLookAtLH(&m_ViewMatrix, &m_Position, &m_Target, &up);

    Renderer::SetViewMatrix(&m_ViewMatrix);

    
    D3DXMatrixPerspectiveFovLH(&projectionMatrix, 1.0f, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 1000.0f);

    Renderer::SetProjectionMatrix(&projectionMatrix);
}

bool Camera::CheckView(D3DXVECTOR3 Position)
{
    D3DXMATRIX vp, invvp;

    vp = m_ViewMatrix * projectionMatrix;
    D3DXMatrixInverse(&invvp, NULL, &vp);

    D3DXVECTOR3 vpos[4];
    D3DXVECTOR3 wpos[4];

    vpos[0] = D3DXVECTOR3(-1.0f,1.0f,1.0f);
    vpos[1] = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
    vpos[2] = D3DXVECTOR3(-1.0f, -1.0f, 1.0f);
    vpos[3] = D3DXVECTOR3(1.0f, -1.0f, 1.0f);

    D3DXVec3TransformCoord(&wpos[0], &vpos[0], &invvp);
    D3DXVec3TransformCoord(&wpos[1], &vpos[1], &invvp);
    D3DXVec3TransformCoord(&wpos[2], &vpos[2], &invvp);
    D3DXVec3TransformCoord(&wpos[3], &vpos[3], &invvp);

    D3DXVECTOR3 v, v1, v2, n;

    v = Position - m_Position;
    //左面判定
    {
        v1 = wpos[0] - m_Position;
        v2 = wpos[2] - m_Position;
        D3DXVec3Cross(&n, &v1, &v2);

        if (D3DXVec3Dot(&n, &v) < 0.0f)
            return false;
    }

    //右面判定
    {
        v1 = wpos[1] - m_Position;
        v2 = wpos[3] - m_Position;
        D3DXVec3Cross(&n, &v1, &v2);

        if (D3DXVec3Dot(&n, &v) > 0.0f)
            return false;
    }
    return true;
}