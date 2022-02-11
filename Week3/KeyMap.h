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

template <class Action>
class KeyMap : public ActionSource<Action> {
private:
    // Controls are held/iterated in insertion order,
    // which may impact the order of actions.
    int index;
    class Control {
    public:
        const int index;

        bool operator==(const Control& other) const;
        bool operator<(const Control& other) const;
        virtual bool isActive() const = 0;
    };

    class KeyboardControl : Control {
    public:
        const ControlType type;
        const unsigned char key;

        KeyboardControl(const int index, const ControlType type, const unsigned char key);
        virtual bool isActive() const;
    };

    class MouseButtonControl : Control {
    public:
        const ControlType type;
        const MouseButton button;

        MouseButtonControl(const int index, const ControlType type, const MouseButton button);
        virtual bool isActive() const;
    };

    class ScrollControl : Control {
        const Scroll direction;
        const int minDelta;
        const int maxDelta;

        ScrollControl(const int index, const Scroll direction, const int minDelta, const int maxDelta);
        virtual bool isActive() const;
    };

    std::map<Control, Action> map;

public:
    KeyMap();
    ~KeyMap();

    // Implement ActionSource<Action>
    virtual std::vector<Action> getActions();

    // KeyMap
    void bindKey(ControlType controlType, unsigned char key, Action action);
    void bindMouseButton(ControlType controlType, MouseButton button, Action action);

    void bindScroll(Scroll direction, int minDelta, int maxDelta, Action action);
    void bindScroll(Scroll direction, int maxDelta, Action action);
    void bindScroll(Scroll direction, Action action);
};
