#pragma once

#include "Dango.h"
#include "DirectXCommon.h"
#include "Player.h"
#include <list>
#include <memory>

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private:
	//テクスチャ一斉生成用関数
	void CreateTexture();

	//団子の生成
	void CreateDango();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	// ビュープロジェクション
	ViewProjection viewProjection_;

	// プレイヤー
	std::unique_ptr<Player> player_ = nullptr;

	// 団子
	std::list<Dango*> dangos_;

	// モデル
	std::unique_ptr<Model> playerModel_; // プレイヤー
	std::unique_ptr<Model> dangoModel_;  // 団子

	// 画像
	uint32_t pinkTex_ = 0u;
	uint32_t whiteTex_ = 0u;
	uint32_t greenTex_ = 0u;
	uint32_t blackTex_ = 0u;

	//団子スポーン感覚
	int DangoSpawnCount_ = 0;
	int DangoSpawnCountMax_ = 60;
};
