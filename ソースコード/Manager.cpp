#include "Main.h"
#include "Manager.h"
#include "Renderer.h"
#include "Input.h"
#include "Scene.h"
#include "Title.h"
#include "Game.h"
#include "Result.h"
#include "Audio.h"

Scene* Manager:: m_Scene = NULL;
void Manager::Init()
{
	Renderer::Init();
	Input::Init();
	Audio::InitMaster();

	
	
	SetScene<Title>();
}


void Manager::Uninit()
{
	
	m_Scene->Uninit();
	delete m_Scene;
	Audio::UninitMaster();
	Input::Uninit();
	Renderer::Uninit();
	
}

void Manager::Update()
{
	Input::Update();
	m_Scene->Update();
}

void Manager::Draw()
{
	Renderer::Begin();
	
	m_Scene->Draw();
	Renderer::End();
}
