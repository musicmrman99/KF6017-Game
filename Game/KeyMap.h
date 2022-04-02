#pragma once

#include <memory>
#include <vector>

#include "Event.h"
#include "EventEmitter.h"
#include "Controls.h"

#include "HasEventHandler.h"

/*
 * Maps Controls to the given type of Events and collates which actions should
 * be run on each frame on whichever actor(s) this KeyMap is set as the
 * EventEmitter for.
 */
class KeyMap final : public EventEmitter {
private:
    // Controls are held/iterated in insertion order, which will impact the
    // order of emitted events.
    std::vector<std::pair<Control::UPtr, EventEmitter::Ptr>> map;
    std::weak_ptr<HasEventHandler> object;

    KeyMap(HasEventHandler::Ptr object);

public:
    using Ptr = std::shared_ptr<KeyMap>;
    using UPtr = std::unique_ptr<KeyMap>;
    using WPtr = std::weak_ptr<KeyMap>;

    static UPtr create(HasEventHandler::Ptr object);

    void bind(Control::UPtr control, EventEmitter::Ptr action);
    void bind(Control* control, EventEmitter* action);

    virtual void emit(std::queue<Event::Ptr>& events) override;
};
