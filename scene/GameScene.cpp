#include "GameScene.h"
#include "Rand.h"
#include "TextureManager.h"
#include <cassert>
#include <stdlib.h>

GameScene::GameScene() {}

GameScene::~GameScene() {
	for (Dango* dango : dangos_) {
		delete dango;
	}
}

void GameScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// ビュープロジェクション初期化
	viewProjection_.Initialize();

	// 乱数の初期化
	SetRandom();

	// テクスチャ生成
	CreateTexture();

	// プレイヤーモデル生成
	playerModel_.reset(Model::CreateFromOBJ("player", true));
	std::vector<Model*> playerModels{
	    playerModel_.get(),
	};

	// プレイヤーの生成と初期化
	player_ = std::make_unique<Player>();
	player_->Initialize(playerModels);

	// 団子モデル生成
	dangoModel_.reset(Model::CreateFromOBJ("Dango", true));
	//団子の生成
	CreateDango();
}

void GameScene::Update() {
	// コントローラー
	// XINPUT_STATE joyState;

	// プレイヤーの更新
	player_->Update();

	// 団子の生成
	if (++DangoSpawnCount_ >= DangoSpawnCountMax_) {
		CreateDango();
		DangoSpawnCount_ = 0;
	}
	// 団子の更新
	for (Dango* dango : dangos_) {
		dango->Update();
	}
	// 団子が画面外に出たら団子リストから取り除く
	dangos_.remove_if([](Dango* dango) {
		if (dango->GetIsDead()) {
			delete dango;

			return true;
		};
		return false;
	});
}

void GameScene::Draw() {
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	// プレイヤーの3D描画
	player_->Draw(viewProjection_);

	// 団子の3D描画
	for (Dango* dango : dangos_) {
		dango->Draw(viewProjection_);
	}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	// プレイヤーの2D描画
	player_->DrawUI();

	// 団子の2D描画
	for (Dango* dango : dangos_) {
		dango->DrawUI();
	}

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::CreateTexture() {
	// テクスチャ
	pinkTex_ = TextureManager::Load("Dango/pink.png");
	greenTex_ = TextureManager::Load("Dango/green.png");
	whiteTex_ = TextureManager::Load("Dango/white.png");
	blackTex_ = TextureManager::Load("Dango/black.png");
}

void GameScene::CreateDango() {
	// 団子モデル生成
	std::vector<Model*> dangoModels{
	    dangoModel_.get(),
	};
	// 団子テクスチャ生成
	std::vector<uint32_t> dangoTextures{pinkTex_, whiteTex_, greenTex_, blackTex_};

	//団子の生成と初期化
	Dango* newDango = new Dango();
	newDango->Initialize(dangoModels, dangoTextures);
	//団子リストの作成
	dangos_.push_back(newDango);
}
