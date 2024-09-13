#pragma once
#include "GameScene.h"
#include "ScoreScene.h"
#include "TitleScene.h"
#include "scene.h"
#include "Transition.h"

class SceneManager {
public:
	void AllInitialize(DirectXCommon* dxCommon);

	void TitleUpdate(Input* input);
	void GameUpdate(Input* input);
	void ScoreUpdate(Input* input);

	void TitleDraw();
	void GameDraw();
	void ScoreDraw();

	void Finalize();

	Scene GetScene() { return scene; }

	void StartSE();
	void StopSE();

	void TitleSceneToGameScene();
	void GameSceneToScoreScene();
	void ScoreSceneToTitleScene();

private:// メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	TitleScene* titleScene = new TitleScene();
	GameScene* gameScene = new GameScene();
	ScoreScene* scoreScene = new ScoreScene();

	//画面遷移
	Transition* transition_ = new Transition();

	Scene scene = TITLE;

	// SE
	uint32_t decisionSe_ = 0u;

	bool isSceneChange = false;

};
