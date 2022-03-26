#include "ObjectEventCreator.h"

ObjectEventFactory::Ptr ObjectEventCreator::objectEventFactory() { return _objectEventFactory; }
ObjectEventFactory::Ptr ObjectEventCreator::objectEventFactory() const { return _objectEventFactory; }
void ObjectEventCreator::setObjectEventFactory(ObjectEventFactory::Ptr objectEventFactory) {
    _objectEventFactory = objectEventFactory;
}
