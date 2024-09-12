#include "Player.h"

// 初期化
void Player::Initialize(const std::vector<Model*>& models, const std::vector<uint32_t>& textures) {
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// モデルの作成
	SetModels(models);
	// テクスチャの生成
	SetTextures(textures);
	currentTex_ = textures_[0];

	// 速度のリセット
	velocity_ = {0.0f, 0.0f, 0.0f};
	// 座標のリセット
	worldTransform_.Initialize();
	worldTransform_.translation_ = {0.0f, -4.0f, -40.0f};

	// タグ付け
	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeIdDef::kPlayer));

	// くっついている団子の数のリセット
	dangoCount_ = 0;

	isLeft = false;
	isRight = false;
}

// 更新
void Player::Update() {
	// ゲームパッドの状態を得る変数(XINPUT)
	XINPUT_STATE joyState;

	// ゲームパッドが接続されているとき
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		// 左スティックの左右の取得
		float stick = (float)joyState.Gamepad.sThumbLX / SHRT_MAX;

		// 左に移動
		if (input_->PushKey(DIK_A) || input_->PushKey(DIK_LEFTARROW) || stick <= -0.3f) {
			velocity_ = {-0.3f, 0.0f, 0.0f};
		}
		// 右に移動
		else if (input_->PushKey(DIK_D) || input_->PushKey(DIK_RIGHTARROW) || stick >= 0.3f) {
			velocity_ = {0.3f, 0.0f, 0.0f};
		} else {
			// 速度をリセット
			velocity_ = {0.0f, 0.0f, 0.0f};
		}
	}
	//接続されていないとき
	else {
		//  左に移動
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
	}

	// 移動
	worldTransform_.translation_ += velocity_;

	// 画面外処理
	worldTransform_.translation_.x = Clamp(worldTransform_.translation_.x, -5.0f, 5.0f);

	// 座標の更新
	worldTransform_.UpdateMatrix();
}

// 3D描画
void Player::Draw(const ViewProjection& viewProjection) { models_[0]->Draw(worldTransform_, viewProjection, currentTex_); }

// 2D描画
void Player::DrawUI() {}

Vector3 Player::GetCenterPosition() const {
	const Vector3 offset = {0.0f, 1.5f, 0.0f};
	Vector3 worldPos = Transform(offset, worldTransform_.matWorld_);
	return worldPos;
}

void Player::OnCollision([[maybe_unused]] Collider* other) {}

void Player::SetParentPlayer(const WorldTransform* parent) { parent = &worldTransform_; }

void Player::Reset() {
	currentTex_ = textures_[0];

	// 速度のリセット
	velocity_ = {0.0f, 0.0f, 0.0f};
	// 座標のリセット
	worldTransform_.translation_ = {0.0f, -4.0f, -40.0f};

	isLeft = false;
	isRight = false;
}
