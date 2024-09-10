#pragma once
#include "Vector3.h"
#include <cstdint>

class Collider {
public:
	virtual ~Collider() = default;

	virtual void OnCollision([[maybe_unused]]Collider* other) {}

	virtual Vector3 GetCenterPosition() const = 0;

	virtual float GetRadius() { return radius; }
	virtual void SetRadius(float rad) { radius = rad; }

	//識別IDを取得
	virtual uint32_t GetTypeID() const { return typeID_; }

	//識別IDを設定
	virtual void SetTypeID(uint32_t typeID) { typeID_ = typeID; }

private:
	float radius = 0.5f;

	//識別ID
	uint32_t typeID_ = 0u;
};
