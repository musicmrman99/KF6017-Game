#include "ObjectFactoryManager.h"

void ObjectFactoryManager::registerFactory(const ObjectType& type, const ObjectFactory& factory) {
	factories.insert({ type, factory });
}

GameObject::Ptr ObjectFactoryManager::create(ObjectSpec::UPtr spec) {
	return factories.at(spec->type)(move(spec));
}
