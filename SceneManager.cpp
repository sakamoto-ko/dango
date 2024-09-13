#include "SceneManager.h"
#include "Rand.h"

void SceneManager::AllInitialize(DirectXCommon* dxCommon) {
	dxCommon_ = dxCommon;
	audio_ = Audio::GetInstance();
	input_ = Input::GetInstance();

	// 乱数の初期化
	SetRandom();

	titleScene->Initialize();
	gameScene->Initialize();
	scoreScene->Initialize();

	transition_->Initialize();

	decisionSe_ = audio_->LoadWave("SE/decision.wav");

	isSceneChange = false;
}

void SceneManager::TitleUpdate(Input* input) {
	// ゲームパッドの状態を得る変数(XINPUT)
	XINPUT_STATE joyState;

	titleScene->Update();
	if (titleScene->GetIsSpace()) {
		// キーボード操作
		if (input->TriggerKey(DIK_SPACE)) {
			StartSE();
			transition_->SetFadeIn(true);
			isSceneChange = true;
		}
		// ゲームパッド操作
		if (Input::GetInstance()->GetJoystickState(0, joyState)) {
			if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A) {
				StartSE();
				transition_->SetFadeIn(true);
				isSceneChange = true;
			}
		}

		transition_->Update();

		if (isSceneChange) {
			if (!transition_->GetFadeOut() && !transition_->GetFadeIn()) {
				TitleSceneToGameScene();
			}
		}
	}
}

void SceneManager::GameUpdate(Input* input) {
	// ゲームパッドの状態を得る変数(XINPUT)
	XINPUT_STATE joyState;

	gameScene->Update();

	if (gameScene->GetIsEnd()) {
		// キーボード操作
		if (input->TriggerKey(DIK_SPACE)) {
			StartSE();
			isSceneChange = true;
		}
		// ゲームパッド操作
		if (Input::GetInstance()->GetJoystickState(0, joyState)) {
			if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A) {
				StartSE();
				isSceneChange = true;
			}
		}

		transition_->Update();

		if (isSceneChange) {
			GameSceneToScoreScene();
		}
	}
}

void SceneManager::ScoreUpdate(Input* input) {
	// ゲームパッドの状態を得る変数(XINPUT)
	XINPUT_STATE joyState;

	scoreScene->Update();

	if (scoreScene->GetIsSpace()) {
		// キーボード操作
		if (input->TriggerKey(DIK_SPACE)) {
			StartSE();
			isSceneChange = true;
		}
		// ゲームパッド操作
		if (Input::GetInstance()->GetJoystickState(0, joyState)) {
			if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A) {
				StartSE();
				isSceneChange = true;
			}
		}

		transition_->Update();

		if (isSceneChange) {
			ScoreSceneToTitleScene();
		}
	}
}

void SceneManager::TitleDraw() {
	titleScene->Draw(transition_);
}

void SceneManager::GameDraw() { gameScene->Draw(transition_); }

void SceneManager::ScoreDraw() { scoreScene->Draw(transition_); }

void SceneManager::Finalize() {
	// 各種解放
	delete gameScene;
	delete titleScene;
	delete scoreScene;
}

void SceneManager::StartSE() { audio_->PlayWave(decisionSe_, false, 1.0f); }

void SceneManager::StopSE() {}

void SceneManager::TitleSceneToGameScene() {
	scene = GAME;
	titleScene->SetIsSpace(false);
	scoreScene->Reset();
	titleScene->StopAudio();
	isSceneChange = false;
}

void SceneManager::GameSceneToScoreScene() {
	scene = SCORE;
	scoreScene->ScoreCalc(gameScene->GetScore());
	gameScene->SetIsEnd(false);
	titleScene->Reset();
	gameScene->StopAudio();
	scoreScene->StartAudio();
	isSceneChange = false;
}

void SceneManager::ScoreSceneToTitleScene() {
	scene = TITLE;
	scoreScene->SetIsSpace(false);
	gameScene->Reset();
	scoreScene->StopAudio();
	titleScene->StartAudio();
	isSceneChange = false;
}
