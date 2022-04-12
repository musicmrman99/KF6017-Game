#include "BasicCollisionSpec.h"

const ObjectType BasicCollisionSpec::BASIC_COLLISION_SPEC;

BasicCollisionSpec::BasicCollisionSpec()
	: ObjectSpec(BASIC_COLLISION_SPEC) {
}

BasicCollisionSpec::UPtr BasicCollisionSpec::create() {
	return UPtr(new BasicCollisionSpec());
}
