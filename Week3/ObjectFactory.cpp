#include "ObjectFactory.h"

void ObjectFactory::registerFactory(const ObjectType& type, const Factory& factory) {
	factories.insert({ type, factory });
}

GameObject::Ptr ObjectFactory::create(ObjectSpec::UPtr spec) {
	return factories.at(spec->type)(move(spec));
}
