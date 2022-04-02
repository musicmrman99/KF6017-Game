#include "CollisionObserver.h"

CollisionModel::WPtr CollisionObserver::collisionModelWPtr() const { return _collisionModel; }
CollisionModel::Ptr CollisionObserver::collisionModel() const { return collisionModelWPtr().lock(); }
void CollisionObserver::updateCollisionModel(CollisionModel::WPtr collisionModel) { _collisionModel = collisionModel; }
