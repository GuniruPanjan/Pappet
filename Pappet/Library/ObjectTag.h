#pragma once

//ゲームオブジェクトのタグ
enum class ObjectTag
{
	Player,               //プレイヤー
	Enemy,                //エネミー(まとめ)
	Attack,               //攻撃
	EnemyAttack,          //プレイヤー以外の攻撃
	Shield,               //プレイヤーの盾
	Search,               //索敵
	EnemySearch,          //プレイヤー以外の索敵範囲
	Item,                 //アイテムの所得範囲
	Rest,                 //休息できる範囲
	BossEnter             //ボス部屋の入口
};
