#include "GameScene.h"
#include "Rand.h"
#include "TextureManager.h"
#include <cassert>
#include <stdlib.h>

GameScene::GameScene() {}

GameScene::~GameScene() {}

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
	player_->SetRadius(1.5f);

	// 団子モデル生成
	dangoModel_.reset(Model::CreateFromOBJ("Dango", true));
	// 団子の生成
	CreateDango();

	// 衝突マネージャの生成と初期化
	collisionManager_ = std::make_unique<CollisionManager>();

	dangoNum = 0;
	dangoCooldown = 0;
}

void GameScene::Update() {
	// コントローラー
	// XINPUT_STATE joyState;

	if (dangoNum < 3) {

		// プレイヤーの更新
		player_->Update();

		// 団子の生成
		if (++DangoSpawnCount_ >= DangoSpawnCountMax_) {
			CreateDango();
			DangoSpawnCount_ = 0;
		}
		for (std::unique_ptr<Dango>& dango : dangos_) {
			// 団子の更新
			dango->Update();
			// 団子がプレイヤーに当たったら
			if (dango->GetIsHit()) {
				// プレイヤーの子としてペアレントに登録
				player_->SetParentPlayer(dango->GetParent());
				// くっついている団子の数に応じて下に移動
				dango->SetWorldPosition(Vector3(player_->GetPosition().x, dangoPos[dangoNum], player_->GetPosition().z));
				dangoNum++;
				dango->SetIsHit(false);
				dango->SetIsDead(true);
			}
			// プレイヤーにくっついているとき
			if (dango->GetIsDead()) {
				dango->SetWorldPosition(Vector3(player_->GetPosition().x, dango->GetPos().y, player_->GetPosition().z));
			}
		}

		// 衝突判定と応答
		CheckAllCollisions();
		// 団子が画面外に出たら団子リストから取り除く
		dangos_.remove_if([](std::unique_ptr<Dango>& dango) {
			if (dango->GetIsOutOfField()) {
				dango.release();

				return true;
			};
			return false;
		});
	}

	// プレイヤーにくっついている団子が3個を超えたら団子リストから取り除く
	else if (dangoNum >= 3) {
		dangoCooldown++;
		if (dangoCooldown >= 60) {
			dangos_.remove_if([](std::unique_ptr<Dango>& dango) {
				if (dango->GetIsDead()) {
					dango.release();

					return true;
				};
				return false;
			});
			dangoNum = 0;
			dangoCooldown = 0;
		}
	}

	else {}
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
	for (std::unique_ptr<Dango>& dango : dangos_) {
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
	for (std::unique_ptr<Dango>& dango : dangos_) {
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

	// 団子の生成と初期化
	std::unique_ptr<Dango> newDango = std::make_unique<Dango>();
	newDango->Initialize(dangoModels, dangoTextures);
	newDango->SetRadius(0.5f);
	// 団子リストの作成
	dangos_.push_back(std::move(newDango));
}

void GameScene::CheckAllCollisions() {
	// 衝突マネージャのリセット
	collisionManager_->Reset();

	// コライダーをリストに登録
	collisionManager_->AddCollider(player_.get());
	// 団子全てについて
	for (const std::unique_ptr<Dango>& dango : dangos_) {
		collisionManager_->AddCollider(dango.get());
	}

	// 衝突判定と応答
	collisionManager_->CheckAllCollision();
}
