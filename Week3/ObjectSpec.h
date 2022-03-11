#pragma once

#include <memory>

#include "Symbol.h"

class ObjectType final : public Symbol {};

struct ObjectSpec {
public:
	using UPtr = std::unique_ptr<ObjectSpec>;

	const ObjectType& type;

	ObjectSpec(const ObjectType& type);
	virtual ~ObjectSpec();
};
