#pragma once
#include "BaseCharacter.h"

class Player : public BaseCharacter {
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

	// 画像のセット
	void SetTextures(const std::vector<uint32_t>& textures) { textures_ = textures; }

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

	void SetViewProjection(const ViewProjection* viewProjection) { viewProjection_ = viewProjection; }

	void SetTmpPos(Vector3 pos) { tmpPos = pos; }
	const Vector3 GetTmpPos() { return tmpPos; }

	void SetIsShakeStart(bool flag) { isShakeStart = flag; }
	void SetIsShakeFinish(bool flag) { isShakeFinish = flag; }
	const bool GetIsShake() { return isShake; }

	void Reset();

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

	// カメラのビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;

	// プレイヤーのワールドトランスフォーム
	WorldTransform worldTransform_;
	// 速度
	Vector3 velocity_;
	// モデル
	std::vector<Model*> models_;

	// 画像
	std::vector<uint32_t> textures_;
	// 現在セットしている画像
	uint32_t currentTex_ = 0u;

	//くっついている団子の数
	int dangoCount_ = 0;

	Vector3 tmpPos{};
	bool isShakeStart = false;
	bool isShake = false;
	bool isShakeFinish = false;

	bool isLeft = false;
	bool isRight = false;
};
