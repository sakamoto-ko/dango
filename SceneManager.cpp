#include "SceneManager.h"

void SceneManager::AllInitialize(DirectXCommon* dxCommon) {
	dxCommon_ = dxCommon;
	audio_ = Audio::GetInstance();
	input_ = Input::GetInstance();

	titleScene->Initialize();
	gameScene->Initialize();
	scoreScene->Initialize();

	decisionSe_ = audio_->LoadWave("SE/decision.wav");
}

void SceneManager::TitleUpdate(Input* input) {
	// ゲームパッドの状態を得る変数(XINPUT)
	XINPUT_STATE joyState;

	titleScene->Update();
	if (titleScene->GetIsSpace()) {
		//キーボード操作
		if (input->TriggerKey(DIK_SPACE)) {
			StartSE();
			scene = GAME;
			titleScene->SetIsSpace(false);
			scoreScene->Reset();
			titleScene->StopAudio();
		}
		//ゲームパッド操作
		if (Input::GetInstance()->GetJoystickState(0, joyState)) {
			if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A) {
				StartSE();
				scene = GAME;
				titleScene->SetIsSpace(false);
				scoreScene->Reset();
				titleScene->StopAudio();
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
			scene = SCORE;
			gameScene->SetIsEnd(false);
			titleScene->Reset();
			gameScene->StopAudio();
			scoreScene->StartAudio();
		}
		// ゲームパッド操作
		if (Input::GetInstance()->GetJoystickState(0, joyState)) {
			if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A) {
				StartSE();
				scene = SCORE;
				gameScene->SetIsEnd(false);
				titleScene->Reset();
				gameScene->StopAudio();
				scoreScene->StartAudio();
			}
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
			scene = TITLE;
			scoreScene->SetIsSpace(false);
			gameScene->Reset();
			scoreScene->StopAudio();
			titleScene->StartAudio();
		}
		// ゲームパッド操作
		if (Input::GetInstance()->GetJoystickState(0, joyState)) {
			if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A) {
				StartSE();
				scene = TITLE;
				scoreScene->SetIsSpace(false);
				gameScene->Reset();
				scoreScene->StopAudio();
				titleScene->StartAudio();
			}
		}
	}
}

void SceneManager::TitleDraw() { titleScene->Draw(); }

void SceneManager::GameDraw() { gameScene->Draw(); }

void SceneManager::ScoreDraw() { scoreScene->Draw(); }

void SceneManager::Finalize() {
	// 各種解放
	delete gameScene;
	delete titleScene;
	delete scoreScene;
}

void SceneManager::StartSE() { audio_->PlayWave(decisionSe_, false, 1.0f); }

void SceneManager::StopSE() {}
