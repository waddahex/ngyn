#pragma once

#include "window.hpp"
#include <unordered_map>

namespace ngyn
{
  struct InputState
  {
    bool held     = false;
    bool pressed  = false;
    bool released = false;
  };

  enum InputOrigin
  {
    Keyboard,
    Mouse,
    Joystick,
    Gamepad
  };

  struct InputKey
  {
    InputOrigin origin;
    int value;
  };

  struct InputAction
  {
    InputState state;
    std::vector<InputKey> keys; // GLFW key codes
  };

  struct InputCreateInfo
  {
    bool usePrintableKeys   = false;
    bool useFunctionalKeys  = false;
    bool useMouseButtons    = false;
    bool useJoystickButtons = false;
    bool useGamepadButtons  = false;
  };

  class Input
  {
    public:
    Input(void) = default;
    Input(InputCreateInfo createInfo);

    std::unordered_map<std::string, InputAction> actions;

    void update(const Window &window);
    bool held(const std::string &actionName);
    bool pressed(const std::string &actionName);
    bool released(const std::string &actionName);

    private:
    std::unordered_map<std::string, InputAction> printableKeys();
    std::unordered_map<std::string, InputAction> functionalKeys();
    std::unordered_map<std::string, InputAction> mouseButtons();
    std::unordered_map<std::string, InputAction> joystickButtons();
    std::unordered_map<std::string, InputAction> gamepadButtons();
  };
};