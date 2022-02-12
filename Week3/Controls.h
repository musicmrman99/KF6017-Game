#pragma once

#include <vector>
#include <functional>

#include "BooleanCombinators.h"

#include "MyInputs.h"

// Type of binding for boolean controls (like key and mouse button presses)
enum class ControlType {
    HOLD,
    PRESS,
    RELEASE
};

/* Control (abstract)
-------------------- */

// An input or set of inputs that can be bound to an action.
class Control {
public:
    virtual bool isActive() const = 0;
};

/* Keyboard Control
-------------------- */

// The state of a key.
class KeyboardControl : public Control {
private:
    const ControlType type;
    const unsigned char key;

public:
    KeyboardControl(const ControlType type, const unsigned char key);
    virtual bool isActive() const;
};

/* Mouse Button Control
-------------------- */

// A mouse button.
enum class MouseButton {
    LEFT,
    RIGHT,
    MIDDLE
};

// The state of a mouse button.
class MouseButtonControl : public Control {
private:
    const ControlType type;
    const MouseButton button;

public:
    MouseButtonControl(const ControlType type, const MouseButton button);
    virtual bool isActive() const;
};

/* Scroll Control
-------------------- */

// The scroll direction to check and how to check it against the min/max delta parameters.
enum class Scroll {
    UP,     // Checks for an absolute amount of upward scrolling
    DOWN,   // Checks for an absolute amount of downward scrolling
    EITHER, // Checks for an absolute amount of scrolling in either direction
    RANGE   // Checks for a directional amount of scrolling in either direction
};

// The difference between the position of the scroll wheel on the last frame and on this frame.
// Allows checking for various commonly used situations.
class ScrollControl : public Control {
private:
    const Scroll direction;
    const int minDelta;
    const int maxDelta;

public:
    ScrollControl(const Scroll direction, const int minDelta, const int maxDelta);

    int minDeltaFor(int minDelta, const Scroll direction);
    int maxDeltaFor(int maxDelta, const Scroll direction);

    virtual bool isActive() const;
};

/* Composite Control
-------------------- */

// Multiple other controls in some pattern (ie. combined in some way).
class CompositeControl : public Control {
public:
    using Combinator = Combinator<std::vector<Control*>, bool>;
    using Predicate = std::function<bool(Control*)>;

private:
    std::vector<Control*> controls;
    Combinator* combinator;

    static Predicate controlActive;

public:
    CompositeControl(const std::vector<Control*>& controls, Combinator* combinator);
    CompositeControl(const std::vector<Control*>& controls);
    ~CompositeControl();

    virtual bool isActive() const;
};
