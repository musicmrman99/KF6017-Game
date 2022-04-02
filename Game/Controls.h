#pragma once

#include <vector>
#include <functional>

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
    using UPtr = std::unique_ptr<Control>;

    virtual bool isActive() const = 0;
    virtual ~Control();
};

/* Keyboard Control
-------------------- */

// The state of a key.
class KeyboardControl final : public Control {
private:
    const ControlType type;
    const unsigned char key;

public:
    using UPtr = std::unique_ptr<KeyboardControl>;

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
class MouseButtonControl final : public Control {
private:
    const ControlType type;
    const MouseButton button;

public:
    using UPtr = std::unique_ptr<MouseButtonControl>;

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
class ScrollControl final : public Control {
private:
    const Scroll direction;
    const int minDelta;
    const int maxDelta;

public:
    using UPtr = std::unique_ptr<ScrollControl>;

    ScrollControl(const Scroll direction, const int minDelta, const int maxDelta);

    int minDeltaFor(int minDelta, const Scroll direction);
    int maxDeltaFor(int maxDelta, const Scroll direction);

    virtual bool isActive() const;
};
