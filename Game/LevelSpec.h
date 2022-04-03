#pragma once

#include "ObjectSpec.h"

#include "ObjectManager.h"

class LevelSpec final : public ObjectSpec {
private:
	LevelSpec(ObjectManager::Ptr objectManager);

public:
	using Ptr = std::shared_ptr<LevelSpec>;
	using UPtr = std::unique_ptr<LevelSpec>;

	static const ObjectType LEVEL_SPEC;

	ObjectManager::Ptr objectManager;

	static UPtr create(ObjectManager::Ptr objectManager);
};
