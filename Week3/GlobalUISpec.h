#pragma once

#include "ObjectSpec.h"

class GlobalUISpec final : public ObjectSpec {
public:
	using Ptr = std::shared_ptr<GlobalUISpec>;
	using UPtr = std::unique_ptr<GlobalUISpec>;

	static const ObjectType GLOBAL_UI;

	GlobalUISpec();
};
