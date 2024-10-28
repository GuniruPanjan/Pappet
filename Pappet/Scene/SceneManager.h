#pragma once
#include "SceneBase.h"
#include<memory>

//class SceneBase;

/// <summary>
/// シーンを管理するクラス
/// </summary>
class SceneManager
{
public:
	//コンストラクタ
	SceneManager();
	//デストラクタ
	virtual ~SceneManager();

	//初期化処理
	void Init();

	//更新処理
	void Update();

	//描画処理
	void Draw();

	//終了処理
	void End();

	//プログラムを処理する判定を返す
	bool GetEnd() { return m_pScene->GetEnd(); }

private:

	//スマートポインタ
	std::shared_ptr<SceneBase> m_pScene;
};




