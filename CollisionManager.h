#pragma once
#include "BaseCharacter.h"

class CollisionManager {
public:
	void CheckCollisionPair(Collider* colliderA, Collider* colliderB);
	void CheckAllCollision();
	void AddCollider(Collider* collider);
	void Reset();

private:
	std::list<Collider*> colliders_;
};
