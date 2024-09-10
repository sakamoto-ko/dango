#include "Player.h"

// 初期化
void Player::Initialize(const std::vector<Model*>& models) {
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// モデルの作成
	SetModels(models);
	// 速度のリセット
	velocity_ = {0.0f, 0.0f, 0.0f};
	// 座標のリセット
	worldTransform_.Initialize();
	worldTransform_.translation_ = {0.0f, -4.0f, -40.0f};

	// タグ付け
	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeIdDef::kPlayer));

	// くっついている団子の数のリセット
	dangoCount_ = 0;
}

// 更新
void Player::Update() {
	// 左に移動
	if (input_->PushKey(DIK_A) || input_->PushKey(DIK_LEFTARROW)) {
		velocity_ = {-0.3f, 0.0f, 0.0f};
	}
	// 右に移動
	else if (input_->PushKey(DIK_D) || input_->PushKey(DIK_RIGHTARROW)) {
		velocity_ = {0.3f, 0.0f, 0.0f};
	} else {
		// 速度をリセット
		velocity_ = {0.0f, 0.0f, 0.0f};
	}

	// 移動
	worldTransform_.translation_ += velocity_;
	// 画面外処理
	worldTransform_.translation_.x = Clamp(worldTransform_.translation_.x, -5.0f, 5.0f);
	// 座標の更新
	worldTransform_.UpdateMatrix();

	//くっついている団子の数が３個を超えたら0に戻す
	if (dangoCount_ >= 3) {
		dangoCount_ = 0;
	}
}

// 3D描画
void Player::Draw(const ViewProjection& viewProjection) {
	// プレイヤーの描画
	models_[0]->Draw(worldTransform_, viewProjection);
}

// 2D描画
void Player::DrawUI() {}

Vector3 Player::GetCenterPosition() const {
	const Vector3 offset = {0.0f, 1.5f, 0.0f};
	Vector3 worldPos = Transform(offset, worldTransform_.matWorld_);
	return worldPos;
}

void Player::OnCollision([[maybe_unused]] Collider* other) {
	dangoCount_++;
}

void Player::SetParentPlayer(const WorldTransform* parent) { 
	parent = &worldTransform_; 
}
