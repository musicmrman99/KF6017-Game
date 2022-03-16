#pragma once

#include "Event.h"
#include "ObjectEventFactory.h"

class ObjectEventEmitter : public EventEmitter {
private:
    ObjectEventFactory::Ptr _objectEventFactory;

public:
    ObjectEventFactory::Ptr objectEventFactory();
    ObjectEventFactory::Ptr objectEventFactory() const;
    void setObjectEventFactory(ObjectEventFactory::Ptr objectEventFactory);
};
