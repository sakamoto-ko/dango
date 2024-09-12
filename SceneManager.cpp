#include "SceneManager.h"

void SceneManager::AllInitialize(DirectXCommon* dxCommon) {
	dxCommon_ = dxCommon;
	input_ = Input::GetInstance();

	titleScene->Initialize();
	gameScene->Initialize();
	scoreScene->Initialize();
}

void SceneManager::TitleUpdate(Input* input) {
	titleScene->Update();
	if (titleScene->GetIsSpace()) {
		if (input->TriggerKey(DIK_SPACE)) {
			scene = GAME;
			titleScene->SetIsSpace(false);
			scoreScene->Reset();
		}
	}
}

void SceneManager::GameUpdate(Input* input) {
	gameScene->Update();
	if (gameScene->GetIsEnd()) {
		if (input->TriggerKey(DIK_SPACE)) {
			scene = SCORE;
			gameScene->SetIsEnd(false);
			titleScene->Reset();
		}
	}
}

void SceneManager::ScoreUpdate(Input* input) {
	scoreScene->Update();
	if (scoreScene->GetIsSpace()) {
		if (input->TriggerKey(DIK_SPACE)) {
			scene = TITLE;
			scoreScene->SetIsSpace(false);
			gameScene->Reset();
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
