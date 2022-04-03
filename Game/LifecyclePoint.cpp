#include "LifecyclePoint.h"

LifecyclePoint::~LifecyclePoint() {}

void LifecyclePoint::objectCreated(GameObject::Ptr object) {}
void LifecyclePoint::objectDestroyed(GameObject::Ptr object) {}
void LifecyclePoint::run() {}
