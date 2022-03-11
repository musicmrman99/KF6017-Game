#include "ObjectFactory.h"

ObjectFactory::Ptr ObjectFactory::_instance = nullptr;
ObjectFactory::Ptr ObjectFactory::instance() {
	if (!_instance) _instance = ObjectFactory::Ptr(new ObjectFactory());
	return _instance;
}

void ObjectFactory::registerFactory(const ObjectType& type, const Factory& factory) {
	instance()->factories.insert({ type, factory });
}

GameObject::Ptr ObjectFactory::create(ObjectSpec::UPtr spec) {
	return instance()->factories.at(spec->type)(move(spec));
}
