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
#if _DEBUG
	MyLibrary::DebugDraw::Clear();
#endif

	game->Update();

	return shared_from_this();  //自身のポインタを返す
}

void SceneGame::Draw()
{
	game->Draw();

#if _DEBUG
	MyLibrary::DebugDraw::Draw();
#endif
}

void SceneGame::End()
{
	game->End();
}

