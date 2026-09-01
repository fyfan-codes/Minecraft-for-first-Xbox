#ifndef SRC_INPUT_H
#define SRC_INPUT_H

#include <cstdint>
#include <cmath>

// Forward-declare XINPUT_GAMEPAD button masks for convenience.
// The nxdk XInput headers define these (XINPUT_GAMEPAD_A, etc.).
// Input::isPressed accepts the same masks.
class Input {
public:
    void init();
    void update();
    // basic accessors
    float leftStickX() const { return left_x; }
    float leftStickY() const { return left_y; }
    float rightStickX() const { return right_x; }
    float rightStickY() const { return right_y; }
    float triggerLeft() const { return trigger_l; }
    float triggerRight() const { return trigger_r; }
    bool isPressed(uint16_t btnMask) const { return (buttons & btnMask) != 0; }

private:
    float left_x = 0.0f;
    float left_y = 0.0f;
    float right_x = 0.0f;
    float right_y = 0.0f;
    float trigger_l = 0.0f;
    float trigger_r = 0.0f;
    uint16_t buttons = 0;
    // raw state store (nxdk XInput)
    void pollController();
};

#endif // SRC_INPUT_H