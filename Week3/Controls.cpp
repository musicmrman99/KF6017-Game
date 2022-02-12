#include "Controls.h"

/* Keyboard Control
-------------------------------------------------- */

KeyboardControl::KeyboardControl(const ControlType type, const unsigned char key)
    : type(type), key(key) {
};

bool KeyboardControl::isActive() const {
    static MyInputs* myInputs = MyInputs::GetInstance();
    switch (type) {
    case ControlType::HOLD: return myInputs->KeyHeld(key);
    case ControlType::PRESS: return myInputs->KeyPressed(key);
    case ControlType::RELEASE: return myInputs->KeyReleased(key);
    }
    return false;
}

/* Mouse Button Control
-------------------------------------------------- */

MouseButtonControl::MouseButtonControl(const ControlType type, const MouseButton button)
    : type(type), button(button) {
}

bool MouseButtonControl::isActive() const {
    static MyInputs* myInputs = MyInputs::GetInstance();
    switch (type) {
    case ControlType::HOLD:
        switch (button) {
        case MouseButton::LEFT: return myInputs->IfMouseLeftDown();
        case MouseButton::RIGHT: return myInputs->IfMouseRightDown();
        case MouseButton::MIDDLE: return myInputs->IfMouseMiddleDown();
        }

    case ControlType::PRESS:
        switch (button) {
        case MouseButton::LEFT: return myInputs->IfMouseNewLeftDown();
        case MouseButton::RIGHT: return myInputs->IfMouseNewRightDown();
        case MouseButton::MIDDLE: return myInputs->IfMouseNewMiddleDown();
        }

    case ControlType::RELEASE:
        switch (button) {
        case MouseButton::LEFT: return myInputs->IfMouseNewLeftUp();
        case MouseButton::RIGHT: return myInputs->IfMouseNewRightUp();
        case MouseButton::MIDDLE: return myInputs->IfMouseNewMiddleUp();
        }
    }
    return false;
}

/* Scroll Control
-------------------------------------------------- */

ScrollControl::ScrollControl(const Scroll direction, const int minDelta, const int maxDelta)
    : direction(direction),
    minDelta(minDeltaFor(minDelta, direction)),
    maxDelta(maxDeltaFor(maxDelta, direction)) {
}

int ScrollControl::minDeltaFor(int minDelta, const Scroll direction) {
    switch (direction) {
    case Scroll::DOWN: minDelta = -abs(minDelta);
    case Scroll::UP: minDelta = abs(minDelta);
    case Scroll::EITHER: minDelta = abs(minDelta); // Must also abs() the actual delta
    case Scroll::RANGE: break;// as-is
    }
    return minDelta;
}

int ScrollControl::maxDeltaFor(int maxDelta, const Scroll direction) {
    switch (direction) {
    case Scroll::DOWN: maxDelta = -abs(maxDelta);
    case Scroll::UP: maxDelta = abs(maxDelta);
    case Scroll::EITHER: maxDelta = abs(maxDelta); // Must also abs() the actual delta
    case Scroll::RANGE: break; // as-is
    }
    return maxDelta;
}

bool ScrollControl::isActive() const {
    static MyInputs* myInputs = MyInputs::GetInstance();

    int delta = myInputs->GetMouseDZ();
    switch (direction) {
    case Scroll::DOWN:
        // eg. -10 < -3 < -1 ("slow-fast upwards")
        return maxDelta <= delta && delta < minDelta;

    case Scroll::UP:
        // eg. 1 < 3 < 10 ("slow-fast downwards")
        return minDelta <= delta && delta < maxDelta;

    case Scroll::EITHER:
        // eg. -10 < 6 < -4 (false) || 4 < 6 < 10 (true) ("mid-fast in either direction")
        delta = abs(delta);
        return minDelta <= delta && delta < maxDelta;

    case Scroll::RANGE:
        // eg. -4 < 2 < 4 ("slow-mid in either direction")
        return minDelta <= delta && delta < maxDelta;
    }

    return false;
}

/* Composite Control
-------------------------------------------------- */

CompositeControl::CompositeControl(const std::vector<Control*>& controls, Combinator* combinator)
    : controls(controls), combinator(combinator) {
}
CompositeControl::CompositeControl(const std::vector<Control*>& controls)
    : CompositeControl(controls, new All<std::vector<Control*>, Predicate>()) {
}

CompositeControl::~CompositeControl() {
    for (Control*& control : controls) {
        if (control) {
            delete control;
            control = nullptr;
        }
    }

    if (combinator) {
        delete combinator;
        combinator = nullptr;
    }
}

bool CompositeControl::isActive() const {
    return combinator->combine(controls);
}

CompositeControl::Predicate CompositeControl::controlActive = [](const Control* control) {
    return control->isActive();
};
