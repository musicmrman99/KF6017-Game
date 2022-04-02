#pragma once

#include <map>
#include <functional>
#include <memory>

#include "ReferenceWrapperUtils.h"

#include "ObjectFactory.h"
#include "GameObject.h"
#include "ObjectSpec.h"

class ObjectFactoryManager {
private:
	using ObjType = std::reference_wrapper<const ObjectType>;
	std::map<ObjType, ObjectFactory, ReferenceWrapperLess<const ObjectType>> factories;

public:
	void registerFactory(const ObjectType& type, const ObjectFactory& factory);
	GameObject::Ptr create(ObjectSpec::UPtr spec);
};
