#include "KeyMap.h"

#include "myinputs.h"

/* Nested Classes
-------------------------------------------------- */

/* KeyMap::Control
-------------------- */

template<class Action>
bool KeyMap<Action>::Control::operator==(const Control& other) const {
    return index == index;
}

template<class Action>
bool KeyMap<Action>::Control::operator<(const Control& other) const {
    return index < index;
}

/* KeyMap::KeyboardControl
-------------------- */

template<class Action>
KeyMap<Action>::KeyboardControl::KeyboardControl(const int index, const ControlType type, const unsigned char key)
    : index(index), type(type), key(key) {
}

template<class Action>
bool KeyMap<Action>::KeyboardControl::isActive() const {
    static MyInputs* myInputs = MyInputs::getInstance();
    switch (type) {
        case KeyMap<Action>::ControlType::HOLD: return myInputs->KeyHeld(key);
        case KeyMap<Action>::ControlType::PRESS: return myInputs->KeyPressed(key);
        case KeyMap<Action>::ControlType::RELEASE: return myInputs->KeyReleased(key);
    }
    return false;
}

/* KeyMap::MouseButtonControl
-------------------- */

template<class Action>
KeyMap<Action>::MouseButtonControl::MouseButtonControl(const int index, const ControlType type, const MouseButton button)
    : index(index), type(type), button(button) {
}

template<class Action>
bool KeyMap<Action>::MouseButtonControl::isActive() const {
    static MyInputs* myInputs = MyInputs::getInstance();
    switch (type) {
    case KeyMap<Action>::ControlType::HOLD:
        switch (button) {
        case KeyMap<Action>::MouseButton::LEFT: return myInputs->IfMouseLeftDown();
        case KeyMap<Action>::MouseButton::RIGHT: return myInputs->IfMouseRightDown();
        case KeyMap<Action>::MouseButton::MIDDLE: return myInputs->IfMouseMiddleDown();
        }

    case KeyMap<Action>::ControlType::PRESS:
        switch (button) {
        case KeyMap<Action>::MouseButton::LEFT: return myInputs->IfMouseNewLeftDown();
        case KeyMap<Action>::MouseButton::RIGHT: return myInputs->IfMouseNewRightDown();
        case KeyMap<Action>::MouseButton::MIDDLE: return myInputs->IfMouseNewMiddleDown();
        }

    case KeyMap<Action>::ControlType::RELEASE:
        switch (button) {
        case KeyMap<Action>::MouseButton::LEFT: return myInputs->IfMouseNewLeftUp();
        case KeyMap<Action>::MouseButton::RIGHT: return myInputs->IfMouseNewRightUp();
        case KeyMap<Action>::MouseButton::MIDDLE: return myInputs->IfMouseNewMiddleUp();
        }
    }
    return false;
}

/* KeyMap::ScrollControl
-------------------- */

template<class Action>
KeyMap<Action>::ScrollControl::ScrollControl(const int index, const Scroll direction, const int minDelta, const int maxDelta)
    : index(index), direction(direction) {

    switch (direction) {
    case KeyMap<Action>::Scroll::DOWN:
        this->minDelta = -abs(minDelta);
        this->maxDelta = -abs(maxDelta);

    case KeyMap<Action>::Scroll::UP:
        this->minDelta = abs(minDelta);
        this->maxDelta = abs(maxDelta);

    case KeyMap<Action>::Scroll::EITHER:
        this->minDelta = abs(minDelta);
        this->maxDelta = abs(maxDelta);
        // Must also abs() the actual delta

    case KeyMap<Action>::Scroll::RANGE:
        this->minDelta = minDelta;
        this->maxDelta = maxDelta;
    }
}

template<class Action>
bool KeyMap<Action>::ScrollControl::isActive() const {
    static MyInputs* myInputs = MyInputs::GetInstance();

    int delta = myInputs->GetMouseDZ();
    switch (direction) {
    case KeyMap<Action>::Scroll::DOWN:
        // eg. -10 < -3 < -1 ("slow-fast upwards")
        return maxDelta <= delta && delta < minDelta;

    case KeyMap<Action>::Scroll::UP:
        // eg. 1 < 3 < 10 ("slow-fast downwards")
        return minDelta <= delta && delta < maxDelta;

    case KeyMap<Action>::Scroll::EITHER:
        // eg. -10 < 6 < -4 (false) || 4 < 6 < 10 (true) ("mid-fast in either direction")
        delta = abs(delta);
        return minDelta <= delta && delta < maxDelta;

    case KeyMap<Action>::Scroll::RANGE:
        // eg. -4 < 2 < 4 ("slow-mid in either direction")
        return minDelta <= delta && delta < maxDelta;
    }
    
    return false;
}

/* KeyMap
-------------------------------------------------- */

template <class Action>
KeyMap<Action>::KeyMap() {}

template<class Action>
KeyMap<Action>::~KeyMap() {}

template <class Action>
std::vector<Action> KeyMap<Action>::getActions() {
    std::vector<Action> actions;
    for (std::pair<Control, Action> mapping : map) {
        if (mapping.first.isActive()) {
            actions.push_back(mapping.second);
        }
    }
    return actions;
}

template<class Action>
void KeyMap<Action>::bindKey(ControlType controlType, unsigned char key, Action action) {
    KeyboardControl control(index, controlType, key);
    map.insert(std::pair<Control, Action>(control, action));
}

template<class Action>
void KeyMap<Action>::bindMouseButton(ControlType controlType, MouseButton button, Action action) {
    MouseButtonControl control(index, controlType, button);
    map.insert(std::pair<Control, Action>(control, action));
}

template<class Action>
void KeyMap<Action>::bindScroll(Scroll direction, int minDelta, int maxDelta, Action action) {
    ScrollControl control(index, direction, minDelta, minDelta);
    map.insert(std::pair<Control, Action>(control, action));
}

template<class Action>
void KeyMap<Action>::bindScroll(Scroll direction, int maxDelta, Action action) {
    KeyMap<Action>::bindScroll(direction, 0, maxDelta, action);
}

template<class Action>
void KeyMap<Action>::bindScroll(Scroll direction, Action action) {
    KeyMap<Action>::bindScroll(direction, 0, action);
}
