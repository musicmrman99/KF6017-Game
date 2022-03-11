#pragma once

#include <map>
#include <functional>
#include <memory>

#include "ReferenceWrapperUtils.h"

#include "GameObject.h"
#include "ObjectSpec.h"

class ObjectFactory {
public:
	using Ptr = std::shared_ptr<ObjectFactory>;
	// No UPtr, as it's a Singleton
	using WPtr = std::weak_ptr<ObjectFactory>;

	using Factory = std::function<GameObject::Ptr(ObjectSpec::UPtr)>;

private:
	using ObjType = std::reference_wrapper<const ObjectType>;

	static ObjectFactory::Ptr _instance;
	static ObjectFactory::Ptr instance();

	std::map<ObjType, Factory, ReferenceWrapperLess<const ObjectType>> factories;

public:
	static void registerFactory(const ObjectType& type, const Factory& factory);
	static GameObject::Ptr create(ObjectSpec::UPtr spec);
};
