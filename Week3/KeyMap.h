#pragma once
#include "ActionSource.h"

#include <map>

enum class ControlType {
    HOLD,
    PRESS,
    RELEASE
};

enum class MouseButton {
    LEFT,
    RIGHT,
    MIDDLE
};

enum class Scroll {
    UP,
    DOWN,
    EITHER,
    RANGE
};

class Control {
private:
    const int index;

public:
    Control(const int index) : index(index) {}
    int getIndex() const {
        return index;
    }

    bool operator==(const Control& other) const {
        return index == other.index;
    }
    bool operator<(const Control& other) const {
        return index < other.index;
    }

    virtual bool isActive() const = 0;
};

class KeyboardControl : public Control {
private:
    const ControlType type;
    const unsigned char key;

public:
    KeyboardControl(const int index, const ControlType type, const unsigned char key)
        : Control(index), type(type), key(key) {
    };
    virtual bool isActive() const {
        static MyInputs* myInputs = MyInputs::GetInstance();
        switch (type) {
        case ControlType::HOLD: return myInputs->KeyHeld(key);
        case ControlType::PRESS: return myInputs->KeyPressed(key);
        case ControlType::RELEASE: return myInputs->KeyReleased(key);
        }
        return false;
    }
};

class MouseButtonControl : public Control {
private:
private:
    const ControlType type;
    const MouseButton button;

public:
    MouseButtonControl(const int index, const ControlType type, const MouseButton button)
        : Control(index), type(type), button(button) {}
    virtual bool isActive() const {
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
};

class ScrollControl : public Control {
private:
    const Scroll direction;
    int minDelta;
    int maxDelta;

public:
    ScrollControl(const int index, const Scroll direction, const int minDelta, const int maxDelta)
        : Control(index), direction(direction) {
        switch (direction) {
        case Scroll::DOWN:
            this->minDelta = -abs(minDelta);
            this->maxDelta = -abs(maxDelta);

        case Scroll::UP:
            this->minDelta = abs(minDelta);
            this->maxDelta = abs(maxDelta);

        case Scroll::EITHER:
            this->minDelta = abs(minDelta);
            this->maxDelta = abs(maxDelta);
            // Must also abs() the actual delta

        case Scroll::RANGE:
            this->minDelta = minDelta;
            this->maxDelta = maxDelta;
        }
    }
    virtual bool isActive() const {
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
};

class CompositeControl : public Control {
public:
    enum class Combinator {
        NONE,      // = 0
        ANY,       // > 0
        ONLY_SOME, // > 0 && < n
        NOT_ALL,   // < n
        ALL,       // = n
    };

private:
    std::vector<Control*> controls;
    Combinator combinator;

public:
    CompositeControl(const int index, const std::vector<Control*>& controls, Combinator combinator)
        : Control(index), controls(controls), combinator(combinator) {
    }
    CompositeControl(const int index, const std::vector<Control*>& controls)
        : CompositeControl(index, controls, Combinator::ALL) {
    }
    virtual bool isActive() const {
        // These are used for different things in different contexts, but have a common pattern.
        bool some = false;
        bool all = true;
        
        switch (combinator) {
        case Combinator::NONE:
            for (Control* control : controls) {
                if (control->isActive()) { all = false; break; }
            }
            return all;

        case Combinator::ANY:
            for (Control* control : controls) {
                if (control->isActive()) { some = true; break; }
            }
            return some;

        case Combinator::ONLY_SOME:
            for (Control* control : controls) {
                if (control->isActive()) some = true;
                if (!control->isActive()) all = false;
                if (some && !all) break;
            }
            return some && !all;

        case Combinator::NOT_ALL:
            for (Control* control : controls) {
                if (!control->isActive()) { some = true; break; }
            }
            return some;

        case Combinator::ALL:
            for (Control* control : controls) {
                if (!control->isActive()) { all = false; break; }
            }
            return all;
        }
    }
};

struct ControlComparator : std::binary_function<const Control*, const Control*, bool> {
    bool operator() (const Control* a, const Control* b) const {
        return *a < *b;
    }
};

template <class Action>
class KeyMap : public ActionSource<Action> {
private:
    // Controls are held/iterated in insertion order,
    // which may impact the order of actions.
    int nextIndex;
    std::map<Control*, Action, ControlComparator> map;

public:
    KeyMap() : nextIndex(0) {}
    ~KeyMap() {
        for (std::pair<Control*, Action> mapping : map) {
            if (mapping.first) {
                delete mapping.first;
            }
        }
    }

    // Implement ActionSource<Action>
    virtual std::vector<Action> getActions() {
        std::vector<Action> actions;
        for (std::pair<Control*, Action> mapping : map) {
            if (mapping.first->isActive()) {
                actions.push_back(mapping.second);
            }
        }
        return actions;
    }

    // KeyMap
    void bindKey(ControlType controlType, unsigned char key, Action action) {
        KeyboardControl* control = new KeyboardControl(nextIndex++, controlType, key);
        map.insert({ control, action });
    }

    void bindMouseButton(ControlType controlType, MouseButton button, Action action) {
        MouseButtonControl* control = new MouseButtonControl(nextIndex++, controlType, button);
        map.insert({ control, action });
    }

    void bindScroll(Scroll direction, int minDelta, int maxDelta, Action action) {
        ScrollControl* control = new ScrollControl(nextIndex++, direction, minDelta, minDelta);
        map.insert({ control, action });
    }
    void bindScroll(Scroll direction, int maxDelta, Action action) {
        KeyMap<Action>::bindScroll(direction, 0, maxDelta, action);
    }
    void bindScroll(Scroll direction, Action action) {
        KeyMap<Action>::bindScroll(direction, 0, action);
    }
};
