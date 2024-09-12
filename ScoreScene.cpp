#include "ScoreScene.h"

void ScoreScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	backGroundTex_ = TextureManager::Load("UI/result.png");
	backGroundSprite_.reset(Sprite::Create(backGroundTex_, {0.0f, 0.0f}));
	backGroundSprite_->SetTextureRect(
	    {
	        0.0f,
	        0.0f,
	    },
	    {1280.0f, 720.0f});
	backGroundSprite_->SetSize({1280.0f, 720.0f});
	backGroundSprite_->SetPosition({0.0f, 0.0f});

	scoreTex_ = TextureManager::Load("UI/score.png");
	scoreSprite_.reset(Sprite::Create(scoreTex_, {0.0f, 0.0f}));
	scoreSprite_->SetTextureRect(
	    {
	        0.0f,
	        0.0f,
	    },
	    {1280.0f, 720.0f});
	scoreSprite_->SetSize({1120.0f, 630.0f});
	scoreSprite_->SetPosition({85.0f, 30.0f});

	backTex_ = TextureManager::Load("UI/spaceback.png");
	backSprite_.reset(Sprite::Create(backTex_, {0.0f, 0.0f}));
	backSprite_->SetTextureRect(
	    {
	        0.0f,
	        0.0f,
	    },
	    {1280.0f, 720.0f});
	backSprite_->SetSize({960.0f, 540.0f});
	backSprite_->SetPosition({150.0f, 300.0f});

	/*numTex_[0] = TextureManager::Load("UI/number/0.png");
	numTex_[1] = TextureManager::Load("UI/number/1.png");
	numTex_[2] = TextureManager::Load("UI/number/2.png");
	numTex_[3] = TextureManager::Load("UI/number/3.png");
	numTex_[4] = TextureManager::Load("UI/number/4.png");
	numTex_[5] = TextureManager::Load("UI/number/5.png");
	numTex_[6] = TextureManager::Load("UI/number/6.png");
	numTex_[7] = TextureManager::Load("UI/number/7.png");
	numTex_[8] = TextureManager::Load("UI/number/8.png");
	numTex_[9] = TextureManager::Load("UI/number/9.png");
	for (int i = 0; i < 10; i++) {
		numSprite_[i].reset(Sprite::Create(numTex_[i], {0.0f, 0.0f}));
		numSprite_[i]->SetTextureRect(
		    {
		        0.0f,
		        0.0f,
		    },
		    {256.0f, 256.0f});
		numSprite_[i]->SetSize({64.0f, 64.0f});
		numSprite_[i]->SetPosition({0.0f, 0.0f});
	}*/

	count = 0;
	isSpace = false;

	BGM_ = audio_->LoadWave("BGM/result.wav");
}

void ScoreScene::Update() {
	// キーディレイ
	if (++count >= 20) {
		isSpace = true;
	}
}

void ScoreScene::Draw() {
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

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	scoreSprite_->Draw();

	backSprite_->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void ScoreScene::Reset() {
	backGroundSprite_->SetPosition({0.0f, 0.0f});
	scoreSprite_->SetPosition({85.0f, 30.0f});
	backSprite_->SetPosition({150.0f, 300.0f});
	count = 0;
	isSpace = false;
}

void ScoreScene::StopAudio() { audio_->StopWave(audioHandle_); }

void ScoreScene::StartAudio() { audioHandle_ = audio_->PlayWave(BGM_, true, 1); }