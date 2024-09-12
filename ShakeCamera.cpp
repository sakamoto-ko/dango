#include "ShakeCamera.h"

// 初期化
void ShakeCamera::Initialize(Vector3 pos) {
	worldTransform_.Initialize();
	//引数でワールド座標を受け取ってワールドトランスフォームに設定
	worldTransform_.translation_ = pos;
	
	tmp = worldTransform_.translation_;

	// ビュープロジェクションの初期化
	viewProjection_.Initialize();
	viewProjection_.farZ = 10000.0f;

	count = 0;
	shake = {0.0f, 0.0f, 0.0f};
}
// 更新
void ShakeCamera::Update() {
	count++;
	shake.x = float(rand() % 10 - 5) * 0.1f;
	shake.y = float(rand() % 10 - 5) * 0.1f;
	worldTransform_.translation_ = Add(worldTransform_.translation_, shake);
	// ワールドトランスフォームのワールド座標再計算
	worldTransform_.UpdateMatrix();
	if (count >= shakeContMax) {
		count = 0;
		isShake = false;
		shake = {0.0f, 0.0f, 0.0f};
		worldTransform_.translation_ = tmp;
		// ワールドトランスフォームのワールド座標再計算
		worldTransform_.UpdateMatrix();
	}

	viewProjection_.UpdateMatrix();
	viewProjection_.TransferMatrix();
}