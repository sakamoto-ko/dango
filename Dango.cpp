#include "Dango.h"

// 初期化
void Dango::Initialize(const std::vector<Model*>& models, const std::vector<uint32_t>& textures) {
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// モデルの作成
	SetModels(models);

	// テクスチャの生成
	SetTextures(textures);

	//ランダムカラー
	int randomColor = rand() % 4;
	currentTex_ = textures_[randomColor];

	// 速度のリセット
	velocity_ = {0.0f, 0.0f, 0.0f};
	//ランダム落下速度
	velocity_.y = float(rand() % 10 + 1);

	// 座標のリセット
	worldTransform_.Initialize();
	worldTransform_.translation_ = {0.0f, 5.0f, -40.0f};
	worldTransform_.scale_ = {0.45f, 0.45f, 0.45f};
	//ランダムX座標スポーン
	worldTransform_.translation_.x = float(rand() % 8 - 4);

	isDead_ = false;
}

// 更新
void Dango::Update() {
	// 画面外に出たら死
	if (worldTransform_.translation_.x >= 5.0f || worldTransform_.translation_.x <= -5.0f || worldTransform_.translation_.y <= -5.0f) {
		isDead_ = true;
	}

	//移動
	// 下方向
	worldTransform_.translation_.y -= velocity_.y * 0.01f;

	// 座標の更新
	worldTransform_.UpdateMatrix();
}

// 3D描画
void Dango::Draw(const ViewProjection& viewProjection) {
	// プレイヤーの描画
	models_[0]->Draw(worldTransform_, viewProjection, currentTex_);
}

// 2D描画
void Dango::DrawUI() {}
