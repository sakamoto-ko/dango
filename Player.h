#pragma once
#include "BaseCharacter.h"

class Player : public BaseCharacter {
public: // 関数
	// 初期化
	void Initialize(const std::vector<Model*>& models);
	// 更新
	void Update();
	// 3D描画
	void Draw(const ViewProjection& viewProjection);
	// 2D描画
	void DrawUI();

	// モデル配列のセット
	void SetModels(const std::vector<Model*>& models) { models_ = models; }

	// 座標の取得
	Vector3 GetPosition() { return worldTransform_.translation_; }
	void SetWorldPosition(Vector3 position) {
		worldTransform_.translation_ = position;
		worldTransform_.UpdateMatrix();
	}

	Vector3 GetCenterPosition() const override;

	//衝突を検出したら呼び出されるコールバック関数
	void OnCollision([[maybe_unused]] Collider* other) override;

	//くっついている団子の数を取得
	int GetDangoCount() { return dangoCount_; }

	//当たった団子をプレイヤーの子としてペアレントする
	void SetParentPlayer(const WorldTransform* parent);

private: // 関数

public:  // 変数
private: // 変数
	//入力
	Input* input_ = nullptr;
	// 音再生
	Audio* audio_ = nullptr;

	// 入力クールタイム
	const int kInputCoolTime = 5;
	int inputCoolTimer_ = 0;

	// プレイヤーのワールドトランスフォーム
	WorldTransform worldTransform_;
	// 速度
	Vector3 velocity_;
	// モデル
	std::vector<Model*> models_;

	//くっついている団子の数
	int dangoCount_ = 0;
};
