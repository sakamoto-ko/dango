#pragma once
#include "GameScene.h"
#include "ScoreScene.h"
#include "TitleScene.h"
#include "scene.h"

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

private:// メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	TitleScene* titleScene = new TitleScene();
	GameScene* gameScene = new GameScene();
	ScoreScene* scoreScene = new ScoreScene();

	Scene scene = TITLE;
};
