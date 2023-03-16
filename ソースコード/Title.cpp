#include "Main.h"
#include "Manager.h"
#include "Renderer.h"
#include "Model.h"
#include "Input.h"
#include "Title.h"
#include "TitleStartButton.h"
#include "TitleLogo.h"
#include "TitleGround.h"
#include "Polygon2D.h"
#include "Fade.h"
#include "Game.h"
#include "FadeIn.h"


void Title::Init()
{
	
	AddGameObject<TitleLogo>(2);
	AddGameObject<TitleStartButton>(2);
	AddGameObject<TitleGround>(1);
	m_TitleCount = 0.0f;

	start = false;
}

void Title::Uninit()
{
	Scene::Uninit();
	
}

void Title::Update()
{
	Scene::Update();

	if (Input::GetKeyTrigger(VK_RETURN))
	{
		AddGameObject<FadeIn>(3);
		start = true;
	}
	if (start == true)
	{
		m_Count++;
	}
	if (m_Count > 70)
	{
		Manager::SetScene<Game>();
		m_Count = 0;
	}
	
}

void Title::Draw()
{
	Scene::Draw();
}