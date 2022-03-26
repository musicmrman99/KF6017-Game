#pragma once

#include "ObjectEventFactory.h"

class ObjectEventCreator {
private:
    ObjectEventFactory::Ptr _objectEventFactory;

public:
    virtual ObjectEventFactory::Ptr objectEventFactory();
    virtual ObjectEventFactory::Ptr objectEventFactory() const;
    virtual void setObjectEventFactory(ObjectEventFactory::Ptr objectEventFactory);
};
