#pragma once
#include <Audio.h>
#include <Input.h>
#include <Model.h>
#include <Sprite.h>
#include <TextureManager.h>
#include <Vector3.h>
#include <ViewProjection.h>
#include <WorldTransform.h>
#include <memory>
#include <vector>

class Dango {

public: // 関数
	// 初期化
	void Initialize(const std::vector<Model*>& models, const std::vector<uint32_t>& textures);
	// 更新
	void Update();
	// 3D描画
	void Draw(const ViewProjection& viewProjection);
	// 2D描画
	void DrawUI();

	// モデル配列のセット
	void SetModels(const std::vector<Model*>& models) { models_ = models; }

	//画像のセット
	void SetTextures(const std::vector<uint32_t>& textures) { textures_ = textures; }

	// 座標の取得
	Vector3 GetPosition() { return worldTransform_.translation_; }
	void SetWorldPosition(Vector3 position) {
		worldTransform_.translation_ = position;
		worldTransform_.UpdateMatrix();
	}

	//死んだかどうかの取得
	bool GetIsDead() const { return isDead_; }

private: // 関数

public:  // 変数

private: // 変数
	// 入力
	Input* input_ = nullptr;
	// 音再生
	Audio* audio_ = nullptr;

	// 入力クールタイム
	const int kInputCoolTime = 5;
	int inputCoolTimer_ = 0;

	// ワールドトランスフォーム
	WorldTransform worldTransform_;
	// 速度
	Vector3 velocity_;
	// モデル
	std::vector<Model*> models_;
	// 画像
	std::vector<uint32_t> textures_;
	// 現在セットしている画像
	uint32_t currentTex_ = 0u;

	// 死んだかどうか
	bool isDead_ = false;
};
