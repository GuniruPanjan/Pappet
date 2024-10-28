#include "SceneGame.h"
#include "SceneTitle.h"

SceneGame::SceneGame()
{
}

SceneGame::~SceneGame()
{
}

void SceneGame::Init()
{
	game->Init();
}

std::shared_ptr<SceneBase> SceneGame::Update()
{
	game->Update();

	return shared_from_this();  //自身のポインタを返す
}

void SceneGame::Draw()
{
	game->Draw();
}

void SceneGame::End()
{
	game->End();
}

