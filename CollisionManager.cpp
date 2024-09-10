#include "CollisionManager.h"

void CollisionManager::CheckCollisionPair(Collider* colliderA, Collider* colliderB) {
	Vector3 posA = colliderA->GetCenterPosition();
	Vector3 posB = colliderB->GetCenterPosition();

	// Vector3 subtract = Subtract(posB, posA);

	// float distance = Length(subtract);

	if (posA.x - 0.15f <= posB.x + colliderB->GetRadius() && 
		posB.x - colliderB->GetRadius() <= posA.x + 0.15f) {
		if (posA.y - 3.0f <= posB.y + colliderB->GetRadius() && 
			posB.y - colliderB->GetRadius() <= posA.y + 2.0f) {
			colliderA->OnCollision(colliderB);
			colliderB->OnCollision(colliderA);
		}
	}
}

void CollisionManager::CheckAllCollision() {
	std::list<Collider*>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end(); ++itrA) {
		Collider* colliderA = *itrA;

		std::list<Collider*>::iterator itrB = itrA;
		itrB++;

		for (; itrB != colliders_.end(); ++itrB) {
			Collider* colliderB = *itrB;

			CheckCollisionPair(colliderA, colliderB);
		}
	}
}

void CollisionManager::AddCollider(Collider* collider) { colliders_.push_back(collider); }

void CollisionManager::Reset() { colliders_.clear(); }
