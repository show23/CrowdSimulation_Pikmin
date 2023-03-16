#include "Main.h"
#include <math.h>
#include "Input.h"
#include "Manager.h"
#include "Renderer.h"
#include "Camera.h"
#include "Gameobject.h"
#include "Fade.h"
#include "Polygon2D.h"
#include "MeshField.h"
#include "Field.h"
#include "Model.h"
#include "TitleLogo.h"
#include "Player.h"
#include "Enemy.h"
#include "Pikmin.h"
#include "BluePikmin.h"
#include "Explosion.h"
#include "Audio.h"
#include "Score.h"
#include "Dolphin.h"
#include "Platform.h"
#include "Line.h"
#include "Sky.h"
#include "One.h"
#include "SetRing.h"
#include "Game.h"
#include "Result.h"

void Game::Init()
{
	AddGameObject<Fade>(3);
	//float x = rand() % 6 + 1;
	Sky::load();
	Pikmin::load();
	BluePikmin::load();
	Enemy::load();
	Platform::load();
	Dolphin::load();
	Line::load();
	SetRing::load();
	One::load();
	
	
	AddGameObject<Camera>(0);
	AddGameObject<MeshField>(1);
	//AddGameObject<FIELD>(1);
	AddGameObject<Player>(1);
	AddGameObject<Enemy>(1)->SetPosition(D3DXVECTOR3(-25.0f, 0.0f, 0.0f));
	
	//Bullet
	AddGameObject<Pikmin>(1)->SetPosition(D3DXVECTOR3(10, 0.0f, -1.0f));
	AddGameObject<Pikmin>(1)->SetPosition(D3DXVECTOR3(15, 0.0f, -10.0f));
	AddGameObject<Pikmin>(1)->SetPosition(D3DXVECTOR3(20, 0.0f, -10.0f));
	AddGameObject<Pikmin>(1)->SetPosition(D3DXVECTOR3(25, 0.0f, -10.0f));
	AddGameObject<Pikmin>(1)->SetPosition(D3DXVECTOR3(30, 0.0f, -10.0f));

	
	AddGameObject<Pikmin>(1)->SetPosition(D3DXVECTOR3(15, 0.0f, -15.0f));
	AddGameObject<Pikmin>(1)->SetPosition(D3DXVECTOR3(20, 0.0f, -15.0f));
	AddGameObject<Pikmin>(1)->SetPosition(D3DXVECTOR3(25, 0.0f, -15.0f));
	AddGameObject<Pikmin>(1)->SetPosition(D3DXVECTOR3(30, 0.0f, -15.0f));


	AddGameObject<Pikmin>(1)->SetPosition(D3DXVECTOR3(15, 0.0f, -20.0f));
	AddGameObject<Pikmin>(1)->SetPosition(D3DXVECTOR3(20, 0.0f, -20.0f));
	AddGameObject<Pikmin>(1)->SetPosition(D3DXVECTOR3(25, 0.0f, -20.0f));
	AddGameObject<Pikmin>(1)->SetPosition(D3DXVECTOR3(30, 0.0f, -20.0f));
	

	AddGameObject<Line>(1);
	AddGameObject<SetRing>(1);
	


	AddGameObject<Sky>(1);
	AddGameObject<Platform>(1)->SetPosition(D3DXVECTOR3(10, 0, 10));
	

	AddGameObject<Dolphin>(1)->SetPosition(D3DXVECTOR3(5, 0, -10));
	
	AddGameObject<Score>(2);
	/*Audio* bgm = AddGameObject<Audio>(2);
	bgm->Load("asset\\audio\\bgm.wav");
	bgm->Play(true);*/
	

	m_FadeCount = 0.0f;
	start = false;
}

void Game::Uninit()
{
	Scene::Uninit();
	BluePikmin::unload();
	Pikmin::unload();
	Enemy::unload();
	SetRing::unload();
	Platform::unload();
	One::unload();

}

void Game::Update()
{
	
	Scene::Update();
	if (Input::GetKeyTrigger(VK_RETURN))
	{
		Manager::SetScene<Result>();
	}
	
}

