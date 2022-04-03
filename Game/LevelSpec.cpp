#include "LevelSpec.h"

const ObjectType LevelSpec::LEVEL_SPEC;

LevelSpec::LevelSpec(ObjectManager::Ptr objectManager)
	: ObjectSpec(LEVEL_SPEC), objectManager(objectManager) {
}

LevelSpec::UPtr LevelSpec::create(ObjectManager::Ptr objectManager) {
	return UPtr(new LevelSpec(objectManager));
}
