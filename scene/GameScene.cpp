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

	// shadow1->SetActive(false);
	// shadow2->SetActive(false);
	// shadow3->SetActive(false);
	// shadow4->SetActive(false);

	// ビュープロジェクション初期化
	viewProjection_.Initialize();

	// テクスチャ生成
	CreateTexture();

	// キューブ
	cubeModel_.reset(Model::CreateFromOBJ("box", true));
	particleModels = {cubeModel_.get()};
	for (int i = 0; i < particleMax; i++) {
		particlePos[i].Initialize();
		particlePos[i].translation_.z = 40.0f;
		particlePos[i].scale_ = {0.2f, 0.2f, 0.2f};
	}

	// プレイヤーモデル生成
	playerModel_.reset(Model::CreateFromOBJ("player", true));
	std::vector<Model*> playerModels{
	    playerModel_.get(),
	};
	// プレイヤーテクスチャ生成
	std::vector<uint32_t> playerTextures{blownTex_};

	// プレイヤーの生成と初期化
	player_ = std::make_unique<Player>();
	player_->Initialize(playerModels, playerTextures);
	player_->SetRadius(1.5f);

	// 団子モデル生成
	dangoModel_.reset(Model::CreateFromOBJ("Dango", true));
	tutuModel_.reset(Model::CreateFromOBJ("tutu", true));
	// 団子の生成
	CreateDango();

	// 衝突マネージャの生成と初期化
	collisionManager_ = std::make_unique<CollisionManager>();

	dangoNum = 0;
	dangoCooldown = 0;

	// 制限時間
	timeLimit_ = 0;

	for (int j = 0; j < 3; j++) {
		dangoDiscrimination[j] = WHITE;
	}
	getDangoBar = 0;

	score = 0;
	isBakudan = false;
	bakudanCount = 0;

	isStart = false;

	// BGM
	BGM_ = audio_->LoadWave("BGM/play.wav");
	// SE
	hitSe_ = audio_->LoadWave("SE/hit.wav"); // 団子とプレイヤーのヒット時SE
	bombSe_ = audio_->LoadWave("SE/bomb.wav"); // 爆弾とプレイヤーのヒット時SE
	yoiSe_ = audio_->LoadWave("SE/yoi.wav"); //よーい
	startSe_ = audio_->LoadWave("SE/start.wav"); //すたーと
}

void GameScene::Update() {
	// コントローラー
	// XINPUT_STATE joyState;

	if (!isStart) {
		startCount++;
		if (startCount <= 1) {
			StartYoiSE();
		}
		if (startCount == 90) {
			StartStartSE();
		}
		if (startCount >= 180) {
			isStart = true;
			StartAudio();
		}
	} else {
		// 制限時間内なら各種Updateを呼び出す
		if (timeLimit_ <= timeLimitMax) {
			// 制限時間用変数をインクリメント
			timeLimit_++;
			TimeCulc();

			// 爆弾が爆発していないなら
			if (!isBakudan) {

				// 団子が三個くっついていないなら各種Updateを呼び出す
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
							// 団子が爆弾だった時
							if (dango->GetDangoColor() == BOM) {
								StartBombSE();
								isBakudan = true;
								currentScoreprite_[0]->SetTextureHandle(numTex_[0]);
								currentScoreprite_[1]->SetTextureHandle(numTex_[5]);
								currentScoreprite_[2]->SetTextureHandle(minusTex_);
								for (int i = 0; i < particleMax; i++) {
									particlePos[i].translation_ = dango->GetPos();
								}
								dango->SetIsHit(false);
								dango->SetIsDead(true);
							}
							// その他
							else {
								StartHitSE();
								// プレイヤーの子としてペアレントに登録
								player_->SetParentPlayer(dango->GetParent());
								// くっついている団子の数に応じて下に移動
								dango->SetWorldPosition(Vector3(player_->GetPosition().x, dangoPos[dangoNum], player_->GetPosition().z));
								dangoDiscrimination[dangoNum] = dango->GetDangoColor();
								dangoNum++;

								for (int i = 0; i < particleMax; i++) {
									particlePos[i].translation_ = dango->GetPos();
								}

								dango->SetIsHit(false);
								dango->SetIsDead(true);
							}
						}
						// プレイヤーにくっついているとき
						if (dango->GetIsDead()) {
							dango->SetWorldPosition(Vector3(player_->GetPosition().x, dango->GetPos().y, player_->GetPosition().z));
						}
					}

					// 衝突判定と応答
					CheckAllCollisions();
				}
				// 団子が三個そろったとき
				else {
					// 団子の種類の判別とポイント付与
					DangoDiscrimination();
				}
			}
			// 団子削除処理総括関数
			DeleteDango();
		}
		// 制限時間がきたら
		else {
			isEnd = true;
			isBakudan = false;
			dangoNum = 0;
		}
	}
}

void GameScene::Draw(Transition* transition) {
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	backGroundSprite_->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	if (isStart) {
		// プレイヤーの3D描画
		player_->Draw(viewProjection_);

		// 団子の3D描画
		for (std::unique_ptr<Dango>& dango : dangos_) {
			dango->Draw(viewProjection_);
		}

		// 爆弾パーティクル
		if (isBakudan) {
			for (int i = 0; i < particleMax; i++) {
				particleModels[0]->Draw(particlePos[i], viewProjection_, redTex_);
			}
		}

		// 団子三個パーティクル
		if (dangoNum >= 3) {
			for (int i = 0; i < particleMax; i++) {
				particleModels[0]->Draw(particlePos[i], viewProjection_, randomTex_[i]);
			}
		}
	}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	if (!isStart) {
		if (startCount < 90) {
			yoiSprite_->Draw();
		} else {
			startSprite_->Draw();
		}
	} else {
		// プレイヤーの2D描画
		player_->DrawUI();

		// 団子の2D描画
		for (std::unique_ptr<Dango>& dango : dangos_) {
			dango->DrawUI();
		}

		if (dangoNum >= 3) {
			currentDangoSprite_[discrimination]->Draw();
			for (int i = 0; i < 3; i++) {
				currentScoreprite_[i]->Draw();
			}
			pointSprite_->Draw();
		}

		if (isBakudan) {
			bombSprite_->Draw();
			for (int i = 0; i < 3; i++) {
				currentScoreprite_[i]->Draw();
			}
			pointSprite_->Draw();
		}

		// 制限時間の表示
		for (int i = 0; i < 3; i++) {
			numSprite_[i]->Draw();
		}
		// 制限時間がきたら
		if (timeLimit_ >= timeLimitMax) {
			timeUpSprite_->Draw();
			nextSprite_->Draw();
		}
	}

	// 画面遷移
	transition->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::Reset() {
	// プレイヤーのリセット
	player_->Reset();

	dangos_.remove_if([](std::unique_ptr<Dango>& dango) {
		dango.release();
		return true;
	});

	// 団子の生成
	CreateDango();

	collisionManager_->Reset();

	// 団子スポーン間隔
	DangoSpawnCount_ = 0;

	dangoNum = 0;
	dangoCooldown = 0;

	// 制限時間
	timeLimit_ = 0;

	// 団子種類判別用補完変数
	for (int i = 0; i < 3; i++) {
		dangoDiscrimination[i] = 0;
	}
	getDangoBar = 0;

	discrimination = {};

	// スコア
	score = 0;

	isBakudan = false;
	bakudanCount = 0;

	isStart = false;
	startCount = 0;

	isEnd = false;
	isYoiEnd = false;
	yoiCount = 0;
}

void GameScene::CreateTexture() {
	// テクスチャ
	redTex_ = TextureManager::Load("Dango/red.png");
	pinkTex_ = TextureManager::Load("Dango/pink.png");
	greenTex_ = TextureManager::Load("Dango/green.png");
	whiteTex_ = TextureManager::Load("Dango/white.png");
	blackTex_ = TextureManager::Load("Dango/black.png");
	blownTex_ = TextureManager::Load("Dango/brown.png");

	tutuTex_ = TextureManager::Load("tutu/tutu.png");

	timeUpTex_ = TextureManager::Load("UI/timeUp.png");
	timeUpSprite_.reset(Sprite::Create(timeUpTex_, {0.0f, 0.0f}));
	timeUpSprite_->SetTextureRect(
	    {
	        0.0f,
	        0.0f,
	    },
	    {1280.0f, 720.0f});
	timeUpSprite_->SetSize({1280.0f, 720.0f});
	timeUpSprite_->SetPosition({0.0f, 0.0f});

	numTex_[0] = TextureManager::Load("UI/number/0.png");
	numTex_[1] = TextureManager::Load("UI/number/1.png");
	numTex_[2] = TextureManager::Load("UI/number/2.png");
	numTex_[3] = TextureManager::Load("UI/number/3.png");
	numTex_[4] = TextureManager::Load("UI/number/4.png");
	numTex_[5] = TextureManager::Load("UI/number/5.png");
	numTex_[6] = TextureManager::Load("UI/number/6.png");
	numTex_[7] = TextureManager::Load("UI/number/7.png");
	numTex_[8] = TextureManager::Load("UI/number/8.png");
	numTex_[9] = TextureManager::Load("UI/number/9.png");
	minusTex_ = TextureManager::Load("UI/number/mainasu.png");
	for (int i = 0; i < 10; i++) {
		numSprite_[i].reset(Sprite::Create(numTex_[i], {0.0f, 0.0f}));
		numSprite_[i]->SetTextureRect(
		    {
		        0.0f,
		        0.0f,
		    },
		    {256.0f, 256.0f});
		numSprite_[i]->SetSize({182.0f, 182.0f});
		numSprite_[i]->SetPosition({200.0f - (float)i * 96.0f, 0.0f});
	}

	scoreTex_ = TextureManager::Load("UI/score.png");
	scoreSprite_.reset(Sprite::Create(scoreTex_, {0.0f, 0.0f}));
	scoreSprite_->SetTextureRect(
	    {
	        0.0f,
	        0.0f,
	    },
	    {1280.0f, 720.0f});
	scoreSprite_->SetSize({1280.0f, 720.0f});
	scoreSprite_->SetPosition({0.0f, 0.0f});

	pointTex_ = TextureManager::Load("UI/point.png");
	pointSprite_.reset(Sprite::Create(pointTex_, {0.0f, 0.0f}));
	pointSprite_->SetTextureRect(
	    {
	        0.0f,
	        0.0f,
	    },
	    {1280.0f, 720.0f});
	pointSprite_->SetSize({960.0f, 540.0f});
	pointSprite_->SetPosition({300.0f, 300.0f});

	normalTex_ = TextureManager::Load("UI/normal.png");
	currentDangoSprite_[0].reset(Sprite::Create(normalTex_, {0.0f, 0.0f}));
	currentDangoSprite_[0]->SetTextureRect(
	    {
	        0.0f,
	        0.0f,
	    },
	    {1280.0f, 720.0f});
	currentDangoSprite_[0]->SetSize({1280.0f, 720.0f});
	currentDangoSprite_[0]->SetPosition({0.0f, 0.0f});

	kusaTex_ = TextureManager::Load("UI/kusa.png");
	currentDangoSprite_[3].reset(Sprite::Create(kusaTex_, {0.0f, 0.0f}));
	currentDangoSprite_[3]->SetTextureRect(
	    {
	        0.0f,
	        0.0f,
	    },
	    {1280.0f, 720.0f});
	currentDangoSprite_[3]->SetSize({1280.0f, 720.0f});
	currentDangoSprite_[3]->SetPosition({0.0f, 0.0f});

	siroTex_ = TextureManager::Load("UI/siro.png");
	currentDangoSprite_[1].reset(Sprite::Create(siroTex_, {0.0f, 0.0f}));
	currentDangoSprite_[1]->SetTextureRect(
	    {
	        0.0f,
	        0.0f,
	    },
	    {1280.0f, 720.0f});
	currentDangoSprite_[1]->SetSize({1280.0f, 720.0f});
	currentDangoSprite_[1]->SetPosition({0.0f, 0.0f});

	sansyokuTex_ = TextureManager::Load("UI/sansyoku.png");
	currentDangoSprite_[4].reset(Sprite::Create(sansyokuTex_, {0.0f, 0.0f}));
	currentDangoSprite_[4]->SetTextureRect(
	    {
	        0.0f,
	        0.0f,
	    },
	    {1280.0f, 720.0f});
	currentDangoSprite_[4]->SetSize({1280.0f, 720.0f});
	currentDangoSprite_[4]->SetPosition({0.0f, 0.0f});

	momoTex_ = TextureManager::Load("UI/momo.png");
	currentDangoSprite_[2].reset(Sprite::Create(momoTex_, {0.0f, 0.0f}));
	currentDangoSprite_[2]->SetTextureRect(
	    {
	        0.0f,
	        0.0f,
	    },
	    {1280.0f, 720.0f});
	currentDangoSprite_[2]->SetSize({1280.0f, 720.0f});
	currentDangoSprite_[2]->SetPosition({0.0f, 0.0f});

	finishTex_ = TextureManager::Load("UI/finish.png");
	finishSprite_.reset(Sprite::Create(finishTex_, {0.0f, 0.0f}));
	finishSprite_->SetTextureRect(
	    {
	        0.0f,
	        0.0f,
	    },
	    {1280.0f, 720.0f});
	finishSprite_->SetSize({1280.0f, 720.0f});
	finishSprite_->SetPosition({0.0f, 0.0f});

	backTex_ = TextureManager::Load("UI/back.png");
	baclSprite_.reset(Sprite::Create(backTex_, {0.0f, 0.0f}));
	baclSprite_->SetTextureRect(
	    {
	        0.0f,
	        0.0f,
	    },
	    {1280.0f, 720.0f});
	baclSprite_->SetSize({1280.0f, 720.0f});
	baclSprite_->SetPosition({0.0f, 0.0f});

	bombTex_ = TextureManager::Load("UI/bomb.png");
	bombSprite_.reset(Sprite::Create(bombTex_, {0.0f, 0.0f}));
	bombSprite_->SetTextureRect(
	    {
	        0.0f,
	        0.0f,
	    },
	    {1280.0f, 720.0f});
	bombSprite_->SetSize({1280.0f, 720.0f});
	bombSprite_->SetPosition({100.0f, 0.0f});

	backGroundTex_ = TextureManager::Load("UI/gamescene.png");
	backGroundSprite_.reset(Sprite::Create(backGroundTex_, {0.0f, 0.0f}));
	backGroundSprite_->SetTextureRect(
	    {
	        0.0f,
	        0.0f,
	    },
	    {1280.0f, 720.0f});
	backGroundSprite_->SetSize({1280.0f, 720.0f});
	backGroundSprite_->SetPosition({0.0f, 0.0f});

	yoiTex_ = TextureManager::Load("UI/yoi.png");
	yoiSprite_.reset(Sprite::Create(yoiTex_, {0.0f, 0.0f}));
	yoiSprite_->SetTextureRect(
	    {
	        0.0f,
	        0.0f,
	    },
	    {1280.0f, 720.0f});
	yoiSprite_->SetSize({1280.0f, 720.0f});
	yoiSprite_->SetPosition({0.0f, 0.0f});

	startTex_ = TextureManager::Load("UI/start.png");
	startSprite_.reset(Sprite::Create(startTex_, {0.0f, 0.0f}));
	startSprite_->SetTextureRect(
	    {
	        0.0f,
	        0.0f,
	    },
	    {1280.0f, 720.0f});
	startSprite_->SetSize({1280.0f, 720.0f});
	startSprite_->SetPosition({20.0f, 105.0f});

	nextTex_ = TextureManager::Load("UI/nextSpace.png");
	nextSprite_.reset(Sprite::Create(nextTex_, {0.0f, 0.0f}));
	nextSprite_->SetTextureRect(
	    {
	        0.0f,
	        0.0f,
	    },
	    {1280.0f, 720.0f});
	nextSprite_->SetSize({1280.0f, 720.0f});
	nextSprite_->SetPosition({0.0f, 210.0f});

	for (int i = 0; i < 4; i++) {
		currentScoreprite_[i].reset(Sprite::Create(numTex_[0], {0.0f, 0.0f}));
		currentScoreprite_[i]->SetTextureRect(
		    {
		        0.0f,
		        0.0f,
		    },
		    {256.0f, 256.0f});
		currentScoreprite_[i]->SetSize({192.0f, 192.0f});
		currentScoreprite_[i]->SetPosition({400.0f - 96.0f * (float)i, 335.0f});
	}
}

void GameScene::CreateDango() {
	// 団子モデル生成
	std::vector<Model*> dangoModels{
	    dangoModel_.get(),
	    tutuModel_.get(),
	};
	// 団子テクスチャ生成
	std::vector<uint32_t> dangoTextures{pinkTex_, whiteTex_, greenTex_, blackTex_, tutuTex_};

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

void GameScene::DeleteDango() {
	// プレイヤーにくっついている団子が3個を超えたら団子リストから取り除く
	if (dangoNum >= 3) {
		dangoCooldown++;
		// 団子三個パーティクル
		PlayParticle();
		if (dangoCooldown >= 60) {
			dangos_.remove_if([](std::unique_ptr<Dango>& dango) {
				if (dango->GetIsDead()) {
					dango.release();

					return true;
				};
				return false;
			});
			GetPoint();

			dangoNum = 0;
			dangoCooldown = 0;
			getDangoBar++;
		}
	}

	// 団子が画面外に出たら団子リストから取り除く
	dangos_.remove_if([](std::unique_ptr<Dango>& dango) {
		if (dango->GetIsOutOfField()) {
			dango.release();

			return true;
		};
		return false;
	});

	// 当たった団子が爆弾なら団子リストから取り除く
	dangos_.remove_if([](std::unique_ptr<Dango>& dango) {
		if (dango->GetIsDead() && dango->GetDangoColor() == BOM) {
			dango.release();

			return true;
		};
		return false;
	});
	if (isBakudan) {
		bakudanCount++;
		// 爆弾パーティクル
		PlayParticle();
		if (bakudanCount >= 60) {
			GetPoint();
			isBakudan = false;
			bakudanCount = 0;
		}
	}
}

void GameScene::DangoDiscrimination() {
	// 三色だんご
	if (IsSanshoku(dangoDiscrimination[0], dangoDiscrimination[1], dangoDiscrimination[2])) {
		discrimination = SANSHOKUDANGO;
		currentScoreprite_[0]->SetTextureHandle(numTex_[0]);
		currentScoreprite_[1]->SetTextureHandle(numTex_[0]);
		currentScoreprite_[2]->SetTextureHandle(numTex_[1]);
	}
	// ピンクだんご
	else if (IsPink(dangoDiscrimination[0], dangoDiscrimination[1], dangoDiscrimination[2])) {
		discrimination = PINKDANGO;
		currentScoreprite_[0]->SetTextureHandle(numTex_[5]);
		currentScoreprite_[1]->SetTextureHandle(numTex_[7]);
		currentScoreprite_[2]->SetTextureHandle(numTex_[0]);
	}
	// 白だんご
	else if (IsWhite(dangoDiscrimination[0], dangoDiscrimination[1], dangoDiscrimination[2])) {
		discrimination = SIRODANGO;
		currentScoreprite_[0]->SetTextureHandle(numTex_[5]);
		currentScoreprite_[1]->SetTextureHandle(numTex_[7]);
		currentScoreprite_[2]->SetTextureHandle(numTex_[0]);
	}
	// 緑だんご
	else if (IsGreen(dangoDiscrimination[0], dangoDiscrimination[1], dangoDiscrimination[2])) {
		discrimination = MIDORIDANGO;
		currentScoreprite_[0]->SetTextureHandle(numTex_[5]);
		currentScoreprite_[1]->SetTextureHandle(numTex_[7]);
		currentScoreprite_[2]->SetTextureHandle(numTex_[0]);
	}
	// ノーマルだんご
	else {
		discrimination = NORMALDANGO;
		currentScoreprite_[0]->SetTextureHandle(numTex_[0]);
		currentScoreprite_[1]->SetTextureHandle(numTex_[5]);
		currentScoreprite_[2]->SetTextureHandle(numTex_[0]);
	}
}

bool GameScene::IsSanshoku(int dango1, int dango2, int dango3) {
	if (dango1 == GREEN && dango2 == WHITE && dango3 == PINK || dango1 == GREEN && dango2 == PINK && dango3 == WHITE || dango1 == PINK && dango2 == GREEN && dango3 == WHITE ||
	    dango1 == PINK && dango2 == WHITE && dango3 == GREEN || dango1 == WHITE && dango2 == PINK && dango3 == GREEN || dango1 == WHITE && dango2 == GREEN && dango3 == PINK) {
		return true;
	} else {
		return false;
	}
}

bool GameScene::IsWhite(int dango1, int dango2, int dango3) {
	if (dango1 == WHITE && dango2 == WHITE && dango3 == WHITE) {
		return true;
	}
	return false;
}

bool GameScene::IsPink(int dango1, int dango2, int dango3) {
	if (dango1 == PINK && dango2 == PINK && dango3 == PINK) {
		return true;
	}
	return false;
}

bool GameScene::IsGreen(int dango1, int dango2, int dango3) {
	if (dango1 == GREEN && dango2 == GREEN && dango3 == GREEN) {
		return true;
	}
	return false;
}

void GameScene::GetPoint() {
	// 爆弾
	if (isBakudan) {
		score += bakudanPoint;
	}
	// 三色だんご
	else if (IsSanshoku(dangoDiscrimination[0], dangoDiscrimination[1], dangoDiscrimination[2])) {
		score += sanshokuPoint;
	}
	// ピンクだんご
	else if (IsPink(dangoDiscrimination[0], dangoDiscrimination[1], dangoDiscrimination[2])) {
		score += isshokuPoint;
	}
	// 白だんご
	else if (IsWhite(dangoDiscrimination[0], dangoDiscrimination[1], dangoDiscrimination[2])) {
		score += isshokuPoint;
	}
	// 緑だんご
	else if (IsGreen(dangoDiscrimination[0], dangoDiscrimination[1], dangoDiscrimination[2])) {
		score += isshokuPoint;
	}
	// ノーマルだんご
	else {
		score += normalPoint;
	}
}

void GameScene::TimeCulc() {
	int tmpNum = timeLimit_ / 60;
	int i = 0;

	while (i < 3) {
		int dight = tmpNum % 10;
		numSprite_[i]->SetTextureHandle(numTex_[dight]);
		i++;
		tmpNum /= 10;
	}
}

void GameScene::PlayParticle() {
	// パーティクルの上限まで表示
	for (int i = 0; i < particleMax; i++) {
		// パーティクルの色をランダムでかえる処理
		if (dangoNum >= 3) {
			int r = rand() % 3 + 1;

			if (r == 1) {
				randomTex_[i] = pinkTex_;
			} else if (r == 2) {
				randomTex_[i] = whiteTex_;
			} else if (r == 3) {
				randomTex_[i] = greenTex_;
			} else {
				randomTex_[i] = pinkTex_;
			}
		}

		// 傾斜角
		float theta = 2.0f * (float)M_PI * (float)i / particleMax;

		// 座標の決定
		particlePos[i].translation_.x += (particleRad * cosf(theta));
		particlePos[i].translation_.y += (particleRad * sinf(theta));
		particlePos[i].UpdateMatrix();
	}
}

void GameScene::StopAudio() { audio_->StopWave(audioHandle_); }

void GameScene::StartAudio() { audioHandle_ = audio_->PlayWave(BGM_, true, 1); }

void GameScene::StartHitSE() { audio_->PlayWave(hitSe_, false, 1.0f); }
void GameScene::StartBombSE() { audio_->PlayWave(bombSe_, false, 2.0f); }
void GameScene::StartYoiSE() { audio_->PlayWave(yoiSe_, false, 2.0f); }
void GameScene::StartStartSE() { audio_->PlayWave(startSe_, false, 2.0f); }

void GameScene::StopSE() {}
