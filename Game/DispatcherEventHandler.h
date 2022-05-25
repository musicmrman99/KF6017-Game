#pragma once

#include <map>

#include "Referencing.h"

#include "Events.h"
#include "EventHandler.h"

// An event handler that maintains a map of event type -> bound method, and runs
// the corresponding bound method when an event must be handled. This allows for
// multiple sources adding to the map.
template <class ActionSource>
class DispatcherEventHandler final : public EventHandler, public Referencing<ActionSource> {
private:
    std::map<std::reference_wrapper<const EventType>, void (ActionSource::*)()> map;

public:
    using UPtr = std::unique_ptr<DispatcherEventHandler<ActionSource>>;

    static UPtr create();

    void set(const EventType& type, void (ActionSource::* handler)());
    virtual void handle(const Event::Ptr event) override;
};

template <class ActionSource>
inline typename DispatcherEventHandler<ActionSource>::UPtr DispatcherEventHandler<ActionSource>::create() {
    return UPtr(new DispatcherEventHandler<ActionSource>());
}

template <class ActionSource>
inline void DispatcherEventHandler<ActionSource>::set(const EventType& type, void(ActionSource::* handler)()) {
    map[type] = handler;
}

template <class ActionSource>
inline void DispatcherEventHandler<ActionSource>::handle(const Event::Ptr event) {
    (this->ref().lock().get()->*map.at(event->type))();
}
