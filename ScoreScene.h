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

class ScoreScene {
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

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	// ビュープロジェクション
	ViewProjection viewProjection_;

	std::unique_ptr<Sprite> backSprite_;
	uint32_t backTex_ = 0u;

	std::unique_ptr<Sprite> scoreSprite_;
	uint32_t scoreTex_ = 0u;

	std::unique_ptr<Sprite> backGroundSprite_;
	uint32_t backGroundTex_ = 0u;

	std::unique_ptr<Sprite> numSprite_[10];
	uint32_t numTex_[10] = {};

	int count = 0;

	int isSpace = false;
};
