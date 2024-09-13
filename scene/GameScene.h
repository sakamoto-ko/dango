#pragma once

#define _USE_MATH_DEFINES
#include "CollisionManager.h"
#include "Dango.h"
#include "DirectXCommon.h"
#include "Player.h"
#include <list>
#include <memory>
#include <cmath>

enum Discrimination {
	NORMALDANGO,
	SIRODANGO,
	PINKDANGO,
	MIDORIDANGO,
	SANSHOKUDANGO,
};

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

	bool GetIsEnd() { return isEnd; }
	void SetIsEnd(bool flag) { isEnd = flag; }

	void Reset();

	void StopAudio();
	void StartAudio();

	void StartSE();
	void StopSE();

	int GetScore() { return score; }

private:
	// テクスチャ一斉生成用関数
	void CreateTexture();

	// 団子の生成
	void CreateDango();

	// 衝突判定と応答
	void CheckAllCollisions();

	// 団子を消す処理の総括
	void DeleteDango();

	// 三個くっついた団子の種類を判別するやつ
	void DangoDiscrimination();

	// 三色団子かどうか
	bool IsSanshoku(int dango1, int dango2, int dango3);

	// 白団子かどうか
	bool IsWhite(int dango1, int dango2, int dango3);

	// ピンク団子かどうか
	bool IsPink(int dango1, int dango2, int dango3);

	// 緑団子かどうか
	bool IsGreen(int dango1, int dango2, int dango3);

	//点数の獲得
	void GetPoint();

	//パーティクル
	void PlayParticle();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	// ビュープロジェクション
	ViewProjection viewProjection_;

	// プレイヤー
	std::unique_ptr<Player> player_ = nullptr;

	// 団子
	std::list<std::unique_ptr<Dango>> dangos_;

	// 衝突マネージャー
	std::unique_ptr<CollisionManager> collisionManager_;

	// モデル
	std::unique_ptr<Model> playerModel_; // プレイヤー
	std::unique_ptr<Model> dangoModel_;  // 団子
	std::unique_ptr<Model> tutuModel_;   // 筒
	std::unique_ptr<Model> cubeModel_;   // Cube

	// 画像
	uint32_t redTex_ = 0u;
	uint32_t pinkTex_ = 0u;
	uint32_t whiteTex_ = 0u;
	uint32_t greenTex_ = 0u;
	uint32_t blackTex_ = 0u;
	uint32_t blownTex_ = 0u;

	std::unique_ptr<Sprite> numSprite_[10];
	uint32_t numTex_[10] = {};

	std::unique_ptr<Sprite> scoreSprite_;
	uint32_t scoreTex_ = 0u;

	std::unique_ptr<Sprite> pointSprite_;
	uint32_t pointTex_ = 0u;

	uint32_t normalTex_ = 0u;
	uint32_t kusaTex_ = 0u;
	uint32_t siroTex_ = 0u;
	uint32_t momoTex_ = 0u;
	uint32_t sansyokuTex_ = 0u;
	std::unique_ptr<Sprite> currentDangoSprite_[5];

	std::unique_ptr<Sprite> startSprite_;
	uint32_t startTex_ = 0u;

	std::unique_ptr<Sprite> finishSprite_;
	uint32_t finishTex_ = 0u;

	std::unique_ptr<Sprite> baclSprite_;
	uint32_t backTex_ = 0u;

	std::unique_ptr<Sprite> timeUpSprite_;
	uint32_t timeUpTex_ = 0u;

	std::unique_ptr<Sprite> bombSprite_;
	uint32_t bombTex_ = 0u;

	std::unique_ptr<Sprite> yoiSprite_;
	uint32_t yoiTex_ = 0u;

	std::unique_ptr<Sprite> nextSprite_;
	uint32_t nextTex_ = 0u;

	uint32_t tutuTex_ = 0u;

	// 団子スポーン間隔
	int DangoSpawnCount_ = 0;
	int DangoSpawnCountMax_ = 60;

	const float dangoPos[3] = {-3.0f, -2.0f, -1.0f};
	int dangoNum = 0;
	int dangoCooldown = 0;

	// 制限時間
	int timeLimit_ = 0;
	const int timeLimitMax = 1800;

	// 団子種類判別用補完変数
	int dangoDiscrimination[3] = {};
	int getDangoBar = 0;

	uint32_t discrimination = {};

	// スコア
	int score = 0;
	const int bakudanPoint = -50;
	const int normalPoint = 50;
	const int isshokuPoint = 75;
	const int sanshokuPoint = 100;
	std::unique_ptr<Sprite> currentScoreprite_[5];

	bool isDiscrimination = false;

	bool isBakudan = false;
	int bakudanCount = 0;

	std::unique_ptr<Sprite> backGroundSprite_;
	uint32_t backGroundTex_ = 0u;

	bool isStart = false;
	int startCount = 0;

	bool isEnd = false;

	// BGM
	uint32_t BGM_ = 0u;
	uint32_t audioHandle_ = 0u;

	// SE
	uint32_t hitSe_ = 0u;

	//パーティクル
	static const int particleMax = 50;
	const float particleRad = 0.3f;
	std::vector<Model*> particleModels;
	WorldTransform particlePos[particleMax]{};
	uint32_t randomTex_[particleMax] = {};
};
