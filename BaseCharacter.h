#pragma once
#include "Collider.h"
#include <Audio.h>
#include <Input.h>
#include <Sprite.h>
#include <TextureManager.h>
#include <Vector3.h>
#include <Model.h>
#include <ViewProjection.h>
#include <WorldTransform.h>
#include <memory>
#include <vector>
#include "CollisionTypeIdDef.h"

class BaseCharacter : public Collider {
public:
	virtual Vector3 GetCenterPosition() const override;

private:
	WorldTransform worldTransform_;
};
