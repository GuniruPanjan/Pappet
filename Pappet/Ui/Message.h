#pragma once
#include "UIBase.h"
#include "Library/MyLibrary.h"

class MessageObject;
class Player;
class Font;

class Message : public UIBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Message();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~Message();

	//データを読み込む初期化
	void DataInit();

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init(float posX, float posY, float posZ, int official, int one, int two, int three, std::shared_ptr<MyLibrary::Physics> physics);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update(Player& player);

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// 文字描画処理
	/// </summary>
	void DrawString();

	/// <summary>
	/// チュートリアル描画
	/// </summary>
	void DrawTutorial(Player& player);

	/// <summary>
	/// 終了処理
	/// </summary>
	void End();

	//処理ができるのかの判断
	bool GetCan() { return m_can; }
	bool SetCan(bool set) { return m_can = set; }

	//メッセージに当たっているかどうか
	bool GetMessageStay();

	//ポジションを返す
	VECTOR GetPos() { return m_centerPos.ConversionToVECTOR(); }

	//メッセージを表示できるかの判断
	bool GetDraw() { return m_draw; }

	//チュートリアル中は止める
	bool GetStop() { return m_stop; }

	//ボタン取得
	int GetLB() { return m_lbButton; }
	int GetRB() { return m_rbButton; }

private:
	XINPUT_STATE m_xpad;                           //パッド入力
	std::shared_ptr<MessageObject> m_pMessage;     //メッセージの判定
	std::shared_ptr<Font> m_pFont;                 //フォント

	MyLibrary::LibVec3 m_centerPos;

	int m_official;       //公式

	int m_effectTime;      //エフェクトの再生時間
	int m_one;             //一つ目
	int m_two;             //二つ目
	int m_three;           //三つ目

	bool m_draw;           //メッセージを表示する
	bool m_can;            //描画処理を実行できるか

	bool m_tutorial[7];    //チュートリアル数
	bool m_stop;           //チュートリアル中は止まるようにする
};

