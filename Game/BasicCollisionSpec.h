#pragma once

#include "ObjectSpec.h"

class BasicCollisionSpec final : public ObjectSpec {
private:
	BasicCollisionSpec();

public:
	using Ptr = std::shared_ptr<BasicCollisionSpec>;
	using UPtr = std::unique_ptr<BasicCollisionSpec>;

	static const ObjectType BASIC_COLLISION_SPEC;

	static UPtr create();
};
