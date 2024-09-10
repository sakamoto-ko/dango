#pragma once
#include "BaseCharacter.h"

class Dango : public BaseCharacter{

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

	const WorldTransform* GetParent() { return worldTransform_.parent_; }

	// 座標の取得
	Vector3 GetPos() { return worldTransform_.translation_; }
	void SetWorldPosition(Vector3 position) {
		worldTransform_.translation_ = position;
		worldTransform_.UpdateMatrix();
	}

	Vector3 GetCenterPosition() const override;

	// プレイヤーに当たったかどうかの取得
	bool GetIsHit() const { return isHit_; }
	void SetIsHit(bool flag) { isHit_ = flag; }
	//画面外に出たかどうかの判定
	bool GetIsOutOfField() const { return isOutOfField_; }
	// 死んだかどうかの取得
	bool GetIsDead() const { return isDead_; }
	void SetIsDead(bool flag) { isDead_ = flag; }

	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision([[maybe_unused]] Collider* other) override;

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

	// プレイヤーに当たったかどうか
	bool isHit_ = false;
	// 画面外に出たかどうか
	bool isOutOfField_ = false;
	// 死んだかどうか
	bool isDead_ = false;
};
