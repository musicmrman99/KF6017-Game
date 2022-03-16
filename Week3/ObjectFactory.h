#pragma once

#include <map>
#include <functional>
#include <memory>

#include "ReferenceWrapperUtils.h"

#include "GameObject.h"
#include "ObjectSpec.h"

class ObjectFactory {
public:
	using Factory = std::function<GameObject::Ptr(ObjectSpec::UPtr)>;

private:
	using ObjType = std::reference_wrapper<const ObjectType>;
	std::map<ObjType, Factory, ReferenceWrapperLess<const ObjectType>> factories;

public:
	void registerFactory(const ObjectType& type, const Factory& factory);
	GameObject::Ptr create(ObjectSpec::UPtr spec);
};
