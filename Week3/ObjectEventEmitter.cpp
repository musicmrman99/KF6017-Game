#include "ObjectEventEmitter.h"

ObjectEventFactory::Ptr ObjectEventEmitter::objectEventFactory() { return _objectEventFactory; }
ObjectEventFactory::Ptr ObjectEventEmitter::objectEventFactory() const { return _objectEventFactory; }
void ObjectEventEmitter::setObjectEventFactory(ObjectEventFactory::Ptr objectEventFactory) {
    _objectEventFactory = objectEventFactory;
}
