#include "TitleScene.h"

void TitleScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	backGroundTex_ = TextureManager::Load("UI/titledango.png");
	backGroundSprite_.reset(Sprite::Create(backGroundTex_, {0.0f, 0.0f}));
	backGroundSprite_->SetTextureRect(
	    {
	        0.0f,
	        0.0f,
	    },
	    {1280.0f, 720.0f});
	backGroundSprite_->SetSize({1280.0f, 720.0f});
	backGroundSprite_->SetPosition({0.0f, 0.0f});

	titleTex_ = TextureManager::Load("UI/title.png");
	titleSprite_.reset(Sprite::Create(titleTex_, {0.0f, 0.0f}));
	titleSprite_->SetTextureRect(
	    {
	        0.0f,
	        0.0f,
	    },
	    {1280.0f, 720.0f});
	titleSprite_->SetSize({1120.0f, 630.0f});
	titleSprite_->SetPosition({85.0f, -60.0f});

	startTex_ = TextureManager::Load("UI/space.png");
	startSprite_.reset(Sprite::Create(startTex_, {0.0f, 0.0f}));
	startSprite_->SetTextureRect(
	    {
	        0.0f,
	        0.0f,
	    },
	    {1280.0f, 720.0f});
	startSprite_->SetSize({960.0f, 540.0f});
	startSprite_->SetPosition({150.0f, 340.0f});

	BGM_ = audio_->LoadWave("BGM/title.wav");
	StartAudio();

	count = 0;
	isSpace = false;
}

void TitleScene::Update() {
	//キーディレイ
	if (++count >= 20) {
		isSpace = true;
	}
}

void TitleScene::Draw() {
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

	titleSprite_->Draw();

	startSprite_->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void TitleScene::Reset() {
	backGroundSprite_->SetPosition({0.0f, 0.0f});
	titleSprite_->SetPosition({85.0f, -60.0f});
	startSprite_->SetPosition({150.0f, 340.0f});
	count = 0;
	isSpace = false;
}

void TitleScene::StopAudio() { audio_->StopWave(audioHandle_); }

void TitleScene::StartAudio() { audioHandle_ = audio_->PlayWave(BGM_, true, 0.8f); }
