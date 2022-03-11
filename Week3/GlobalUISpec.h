#pragma once

#include "ObjectSpec.h"

class GlobalUISpec final : public ObjectSpec {
public:
	using UPtr = std::unique_ptr<GlobalUISpec>;

	static const ObjectType GLOBAL_UI;

	GlobalUISpec();
};
