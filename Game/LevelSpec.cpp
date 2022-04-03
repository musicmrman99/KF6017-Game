#include "LevelSpec.h"

const ObjectType LevelSpec::LEVEL_SPEC;

LevelSpec::LevelSpec(ObjectManager::WPtr objectManager)
	: ObjectSpec(LEVEL_SPEC), objectManager(objectManager) {
}

LevelSpec::UPtr LevelSpec::create(ObjectManager::WPtr objectManager) {
	return UPtr(new LevelSpec(objectManager));
}
