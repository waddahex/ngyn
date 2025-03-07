#pragma once

#include "../../ngynpch.hpp"

#include "window.hpp"

namespace ngyn
{
  struct InputState
  {
    bool held       = false;
    bool pressed    = false;
    bool released   = false;
    float intensity = 0.0f; // For gamepad axes
  };

  enum InputKeyType
  {
    Keyboard,
    Mouse,
    GamepadButton,
    GamepadAxis,
  };

  enum class InputActionType
  {
    Keyboard,
    Mouse,
    Gamepad,
    Mixed
  };

  struct InputKey
  {
    InputKeyType origin;
    int value;
  };

  struct InputAction
  {
    std::string name;
    InputActionType type;
    std::vector<InputKey> keys; // GLFW key codes
  };

  struct InputCreateInfo
  {
    bool useKeyboard  = true;
    bool useMouse     = true;
    bool useGamepads  = true;
  };

  class Input
  {
    public:
    Input(InputCreateInfo createInfo = InputCreateInfo{});

    void update(const Window &window);

    bool held(const std::string &actionName);
    bool pressed(const std::string &actionName);
    bool released(const std::string &actionName);
    int intensity(const std::string &actionName);
    InputState state(const std::string &actionName);

    bool hasJoystick(int index);
    
    private:
    std::vector<InputAction> printableKeys();
    std::vector<InputAction> functionalKeys();
    std::vector<InputAction> mouseButtons();
    std::vector<InputAction> gamepadButtons();
    
    std::vector<InputAction> actions;
    std::unordered_map<std::string, InputState> states;
    static std::vector<int> connectedJoysticks;
    
    static void joystick_callback(int jid, int event);
    void addConnectedJoysticks();
  };
};