#pragma once
#include <Audio.h>
#include <Input.h>
#include <Sprite.h>
#include <TextureManager.h>
#include <Vector3.h>
#include <Model.h>
#include <ViewProjection.h>
#include <WorldTransform.h>
#include "DirectXCommon.h"

class TitleScene {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	const bool GetIsSpace() { return isSpace; }
	void SetIsSpace(bool flag) { isSpace = flag; }

	void Reset();

	void StopAudio();
	void StartAudio();

private:// メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	// ビュープロジェクション
	ViewProjection viewProjection_;

	std::unique_ptr<Sprite> startSprite_;
	uint32_t startTex_ = 0u;

	std::unique_ptr<Sprite> titleSprite_;
	uint32_t titleTex_ = 0u;

	std::unique_ptr<Sprite> backGroundSprite_;
	uint32_t backGroundTex_ = 0u;

	int count = 0;

	int isSpace = false;

	// BGM
	uint32_t BGM_ = 0u;
	uint32_t audioHandle_ = 0u;
};
