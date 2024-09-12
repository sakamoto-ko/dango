#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"

class ShakeCamera {
private:
	// ビュープロジェクション
	ViewProjection viewProjection_;
	// ワールドトランスフォーム
	WorldTransform worldTransform_;

	bool isShake = false;
	Vector3 tmp;
	int count = 0;
	const int shakeContMax = 60;
	Vector3 shake;

public:
	// 初期化
	void Initialize(Vector3 pos);
	// 更新
	void Update();
	const ViewProjection& GetViewProjection() { return viewProjection_; }
	const WorldTransform& GetWorldTransform() { return worldTransform_; }

	const bool GetIsShake() { return isShake; }
	void SetIsShake(bool flag) { isShake = flag; }
};
