

#include "Main.h"
#include "Manager.h"
#include "Renderer.h"
#include "Model.h"
#include "Input.h"
#include "Result.h"
#include "ResultPolygon2D.h"
#include "Polygon2D.h"
#include "Game.h"
#include "Title.h"


void Result::Init()
{
	AddGameObject<ResultPolygon2D>(2);
}

void Result::Uninit()
{
	Scene::Uninit();
}

void Result::Update()
{
	Scene::Update();

	if (Input::GetKeyTrigger(VK_RETURN))
	{
		Manager::SetScene<Title>();
	}
}

void Result::Draw()
{
	Scene::Draw();
}