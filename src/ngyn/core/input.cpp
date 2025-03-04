#include "input.hpp"

using namespace ngyn;

std::vector<int> Input::connectedJoysticks;

Input::Input(InputCreateInfo createInfo)
{
  if(createInfo.useKeyboard)
  {
    auto printableKeysMap = printableKeys();
    this->actions.insert(this->actions.end(), printableKeysMap.begin(), printableKeysMap.end());

    auto functionalKeysMap = functionalKeys();
    this->actions.insert(this->actions.end(), functionalKeysMap.begin(), functionalKeysMap.end());
  }

  if(createInfo.useMouse)
  {
    auto mouseButtonsMap = mouseButtons();
    this->actions.insert(this->actions.end(), mouseButtonsMap.begin(), mouseButtonsMap.end());
  }

  if(createInfo.useGamepads)
  {
    addConnectedJoysticks();
    glfwSetJoystickCallback(joystick_callback);
    auto gamepadButtonsMap = gamepadButtons();
    this->actions.insert(this->actions.end(), gamepadButtonsMap.begin(), gamepadButtonsMap.end());
  }
}

void Input::update(const Window &window)
{
  for(auto action : this->actions)
  {
    if(action.type == InputActionType::Gamepad)
    {
      for(auto jid : connectedJoysticks)
      {
        GLFWgamepadstate gamepadState;
        int result = glfwGetGamepadState(jid, &gamepadState);

        if(!result)
        {
          continue;
        }

        auto key = action.keys[0];

        std::string name = action.name + "_" + std::to_string(jid);
        auto &state = this->states[name];

        auto keyState = key.origin == InputKeyType::GamepadButton ?
          gamepadState.buttons[key.value] :
          gamepadState.axes[key.value];

        state.intensity = keyState;

        if(std::round(std::abs(keyState)) == GLFW_PRESS)
        {
          if(!state.pressed && !state.held)
          {
            state.released = false;
            state.pressed = true;
            state.held = true;
          }
          else
          {
            state.released = false;
            state.pressed = false;
            state.held = true;
          }
        }
        else if(std::round(std::abs(keyState)) == GLFW_RELEASE)
        {
          if(state.pressed || state.held)
          {
            state.released = true;
          }

          state.held = false;
          state.pressed = false;
        }
      }

      continue;
    }

    if(action.keys.size() == 0)
    {
      continue;
    }

    auto &state = this->states[action.name];
    state.released = false;

    auto key = action.keys[0];

    int keyState = key.origin == InputKeyType::Keyboard ?
      glfwGetKey(window.handle, key.value) :
      glfwGetMouseButton(window.handle, key.value);
    
    if(keyState == GLFW_PRESS)
    {
      if(!state.pressed && !state.held)
      {
        state.released = false;
        state.pressed = true;
        state.held = true;
      }
      else
      {
        state.released = false;
        state.pressed = false;
        state.held = true;
      }
    }
    else if(keyState == GLFW_RELEASE)
    {
      if(state.pressed || state.held)
      {
        state.released = true;
      }

      state.held = false;
      state.pressed = false;
    }
  }
}

bool Input::held(const std::string &actionName)
{
  if(this->states.find(actionName) != this->states.end())
  {
    return this->states[actionName].held;
  }
  return false;
}

bool Input::pressed(const std::string &actionName)
{
  if(this->states.find(actionName) != this->states.end())
  {
    return this->states[actionName].pressed;
  }
  return false;
}

bool Input::released(const std::string &actionName)
{
  if(this->states.find(actionName) != this->states.end())
  {
    return this->states[actionName].released;
  }
  return false;
}

int Input::intensity(const std::string &actionName)
{
  if(this->states.find(actionName) != this->states.end())
  {
    return this->states[actionName].intensity;
  }
  return 0;
}

InputState Input::state(const std::string &actionName)
{
  if(this->states.find(actionName) != this->states.end())
  {
    return this->states[actionName];
  }
  return InputState();
}

bool ngyn::Input::hasJoystick(int index)
{
  return std::find(connectedJoysticks.begin(), connectedJoysticks.end(), index) != connectedJoysticks.end();
}

std::vector<InputAction> Input::printableKeys()
{
  return {
    { "KEY_SPACE",         InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_SPACE } } },
    { "KEY_APOSTROPHE",    InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_APOSTROPHE } } },
    { "KEY_COMMA",         InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_COMMA } } },
    { "KEY_MINUS",         InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_MINUS } } },
    { "KEY_PERIOD",        InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_PERIOD } } },
    { "KEY_SLASH",         InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_SLASH } } },
    { "KEY_0",             InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_0 } } },
    { "KEY_1",             InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_1 } } },
    { "KEY_2",             InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_2 } } },
    { "KEY_3",             InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_3 } } },
    { "KEY_4",             InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_4 } } },
    { "KEY_5",             InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_5 } } },
    { "KEY_6",             InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_6 } } },
    { "KEY_7",             InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_7 } } },
    { "KEY_8",             InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_8 } } },
    { "KEY_9",             InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_9 } } },
    { "KEY_SEMICOLON",     InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_SEMICOLON } } },
    { "KEY_EQUAL",         InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_EQUAL } } },
    { "KEY_A",             InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_A } } },
    { "KEY_B",             InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_B } } },
    { "KEY_C",             InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_C } } },
    { "KEY_D",             InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_D } } },
    { "KEY_E",             InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_E } } },
    { "KEY_F",             InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_F } } },
    { "KEY_G",             InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_G } } },
    { "KEY_H",             InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_H } } },
    { "KEY_I",             InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_I } } },
    { "KEY_J",             InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_J } } },
    { "KEY_K",             InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_K } } },
    { "KEY_L",             InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_L } } },
    { "KEY_M",             InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_M } } },
    { "KEY_N",             InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_N } } },
    { "KEY_O",             InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_O } } },
    { "KEY_P",             InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_P } } },
    { "KEY_Q",             InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_Q } } },
    { "KEY_R",             InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_R } } },
    { "KEY_S",             InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_S } } },
    { "KEY_T",             InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_T } } },
    { "KEY_U",             InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_U } } },
    { "KEY_V",             InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_V } } },
    { "KEY_W",             InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_W } } },
    { "KEY_X",             InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_X } } },
    { "KEY_Y",             InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_Y } } },
    { "KEY_Z",             InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_Z } } },
    { "KEY_LEFT_BRACKET",  InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_LEFT_BRACKET } } },
    { "KEY_BACKSLASH",     InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_BACKSLASH } } },
    { "KEY_RIGHT_BRACKET", InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_RIGHT_BRACKET } } },
    { "KEY_GRAVE_ACCENT",  InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_GRAVE_ACCENT } } },
    { "KEY_WORLD_1",       InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_WORLD_1 } } },
    { "KEY_WORLD_2",       InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_WORLD_2 } } },
  };
}

std::vector<InputAction> Input::functionalKeys()
{
  return {
    { "KEY_ESCAPE",         InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_ESCAPE } } },
    { "KEY_ENTER",          InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_ENTER } } },
    { "KEY_TAB",            InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_TAB } } },
    { "KEY_BACKSPACE",      InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_BACKSPACE } } },
    { "KEY_INSERT",         InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_INSERT } } },
    { "KEY_DELETE",         InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_DELETE } } },
    { "KEY_RIGHT",          InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_RIGHT } } },
    { "KEY_LEFT",           InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_LEFT } } },
    { "KEY_DOWN",           InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_DOWN } } },
    { "KEY_UP",             InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_UP } } },
    { "KEY_PAGE_UP",        InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_PAGE_UP } } },
    { "KEY_PAGE_DOWN",      InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_PAGE_DOWN } } },
    { "KEY_HOME",           InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_HOME } } },
    { "KEY_END",            InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_END } } },
    { "KEY_CAPS_LOCK",      InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_CAPS_LOCK } } },
    { "KEY_SCROLL_LOCK",    InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_SCROLL_LOCK } } },
    { "KEY_NUM_LOCK",       InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_NUM_LOCK } } },
    { "KEY_PRINT_SCREEN",   InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_PRINT_SCREEN } } },
    { "KEY_PAUSE",          InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_PAUSE } } },
    { "KEY_F1",             InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_F1 } } },
    { "KEY_F2",             InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_F2 } } },
    { "KEY_F3",             InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_F3 } } },
    { "KEY_F4",             InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_F4 } } },
    { "KEY_F5",             InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_F5 } } },
    { "KEY_F6",             InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_F6 } } },
    { "KEY_F7",             InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_F7 } } },
    { "KEY_F8",             InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_F8 } } },
    { "KEY_F9",             InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_F9 } } },
    { "KEY_F10",            InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_F10 } } },
    { "KEY_F11",            InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_F11 } } },
    { "KEY_F12",            InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_F12 } } },
    { "KEY_F13",            InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_F13 } } },
    { "KEY_F14",            InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_F14 } } },
    { "KEY_F15",            InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_F15 } } },
    { "KEY_F16",            InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_F16 } } },
    { "KEY_F17",            InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_F17 } } },
    { "KEY_F18",            InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_F18 } } },
    { "KEY_F19",            InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_F19 } } },
    { "KEY_F20",            InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_F20 } } },
    { "KEY_F21",            InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_F21 } } },
    { "KEY_F22",            InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_F22 } } },
    { "KEY_F23",            InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_F23 } } },
    { "KEY_F24",            InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_F24 } } },
    { "KEY_F25",            InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_F25 } } },
    { "KEY_KP_0",           InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_KP_0 } } },
    { "KEY_KP_1",           InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_KP_1 } } },
    { "KEY_KP_2",           InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_KP_2 } } },
    { "KEY_KP_3",           InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_KP_3 } } },
    { "KEY_KP_4",           InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_KP_4 } } },
    { "KEY_KP_5",           InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_KP_5 } } },
    { "KEY_KP_6",           InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_KP_6 } } },
    { "KEY_KP_7",           InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_KP_7 } } },
    { "KEY_KP_8",           InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_KP_8 } } },
    { "KEY_KP_9",           InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_KP_9 } } },
    { "KEY_KP_DECIMAL",     InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_KP_DECIMAL } } },
    { "KEY_KP_DIVIDE",      InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_KP_DIVIDE } } },
    { "KEY_KP_MULTIPLY",    InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_KP_MULTIPLY } } },
    { "KEY_KP_SUBTRACT",    InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_KP_SUBTRACT } } },
    { "KEY_KP_ADD",         InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_KP_ADD } } },
    { "KEY_KP_ENTER",       InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_KP_ENTER } } },
    { "KEY_KP_EQUAL",       InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_KP_EQUAL } } },
    { "KEY_LEFT_SHIFT",     InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_LEFT_SHIFT } } },
    { "KEY_LEFT_CONTROL",   InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_LEFT_CONTROL } } },
    { "KEY_LEFT_ALT",       InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_LEFT_ALT } } },
    { "KEY_LEFT_SUPER",     InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_LEFT_SUPER } } },
    { "KEY_RIGHT_SHIFT",    InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_RIGHT_SHIFT } } },
    { "KEY_RIGHT_CONTROL",  InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_RIGHT_CONTROL } } },
    { "KEY_RIGHT_ALT",      InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_RIGHT_ALT } } },
    { "KEY_RIGHT_SUPER",    InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_RIGHT_SUPER } } },
    { "KEY_MENU",           InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_MENU } } },
    { "KEY_LAST",           InputActionType::Keyboard, { { InputKeyType::Keyboard, GLFW_KEY_LAST } } },
  };
}

std::vector<InputAction> Input::mouseButtons()
{
  return {
    { "MOUSE_BUTTON_1",       InputActionType::Mouse, { { InputKeyType::Mouse, GLFW_MOUSE_BUTTON_1 } } },
    { "MOUSE_BUTTON_2",       InputActionType::Mouse, { { InputKeyType::Mouse, GLFW_MOUSE_BUTTON_2 } } },
    { "MOUSE_BUTTON_3",       InputActionType::Mouse, { { InputKeyType::Mouse, GLFW_MOUSE_BUTTON_3 } } },
    { "MOUSE_BUTTON_4",       InputActionType::Mouse, { { InputKeyType::Mouse, GLFW_MOUSE_BUTTON_4 } } },
    { "MOUSE_BUTTON_5",       InputActionType::Mouse, { { InputKeyType::Mouse, GLFW_MOUSE_BUTTON_5 } } },
    { "MOUSE_BUTTON_6",       InputActionType::Mouse, { { InputKeyType::Mouse, GLFW_MOUSE_BUTTON_6 } } },
    { "MOUSE_BUTTON_7",       InputActionType::Mouse, { { InputKeyType::Mouse, GLFW_MOUSE_BUTTON_7 } } },
    { "MOUSE_BUTTON_8",       InputActionType::Mouse, { { InputKeyType::Mouse, GLFW_MOUSE_BUTTON_8 } } },
    { "MOUSE_BUTTON_LAST",    InputActionType::Mouse, { { InputKeyType::Mouse, GLFW_MOUSE_BUTTON_LAST } } },
    { "MOUSE_BUTTON_LEFT",    InputActionType::Mouse, { { InputKeyType::Mouse, GLFW_MOUSE_BUTTON_LEFT } } },
    { "MOUSE_BUTTON_RIGHT",   InputActionType::Mouse, { { InputKeyType::Mouse, GLFW_MOUSE_BUTTON_RIGHT } } },
    { "MOUSE_BUTTON_MIDDLE",  InputActionType::Mouse, { { InputKeyType::Mouse, GLFW_MOUSE_BUTTON_MIDDLE } } },
  };
}

std::vector<InputAction> Input::gamepadButtons()
{
  return {
    { "GAMEPAD_BUTTON_A",             InputActionType::Gamepad, { { InputKeyType::GamepadButton, GLFW_GAMEPAD_BUTTON_A } } },
    { "GAMEPAD_BUTTON_B",             InputActionType::Gamepad, { { InputKeyType::GamepadButton, GLFW_GAMEPAD_BUTTON_B } } },
    { "GAMEPAD_BUTTON_X",             InputActionType::Gamepad, { { InputKeyType::GamepadButton, GLFW_GAMEPAD_BUTTON_X } } },
    { "GAMEPAD_BUTTON_Y",             InputActionType::Gamepad, { { InputKeyType::GamepadButton, GLFW_GAMEPAD_BUTTON_Y } } },
    { "GAMEPAD_BUTTON_LEFT_BUMPER",   InputActionType::Gamepad, { { InputKeyType::GamepadButton, GLFW_GAMEPAD_BUTTON_LEFT_BUMPER } } },
    { "GAMEPAD_BUTTON_RIGHT_BUMPER",  InputActionType::Gamepad, { { InputKeyType::GamepadButton, GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER } } },
    { "GAMEPAD_BUTTON_BACK",          InputActionType::Gamepad, { { InputKeyType::GamepadButton, GLFW_GAMEPAD_BUTTON_BACK } } },
    { "GAMEPAD_BUTTON_START",         InputActionType::Gamepad, { { InputKeyType::GamepadButton, GLFW_GAMEPAD_BUTTON_START } } },
    { "GAMEPAD_BUTTON_GUIDE",         InputActionType::Gamepad, { { InputKeyType::GamepadButton, GLFW_GAMEPAD_BUTTON_GUIDE } } },
    { "GAMEPAD_BUTTON_LEFT_THUMB",    InputActionType::Gamepad, { { InputKeyType::GamepadButton, GLFW_GAMEPAD_BUTTON_LEFT_THUMB } } },
    { "GAMEPAD_BUTTON_RIGHT_THUMB",   InputActionType::Gamepad, { { InputKeyType::GamepadButton, GLFW_GAMEPAD_BUTTON_RIGHT_THUMB } } },
    { "GAMEPAD_BUTTON_DPAD_UP",       InputActionType::Gamepad, { { InputKeyType::GamepadButton, GLFW_GAMEPAD_BUTTON_DPAD_UP } } },
    { "GAMEPAD_BUTTON_DPAD_RIGHT",    InputActionType::Gamepad, { { InputKeyType::GamepadButton, GLFW_GAMEPAD_BUTTON_DPAD_RIGHT } } },
    { "GAMEPAD_BUTTON_DPAD_DOWN",     InputActionType::Gamepad, { { InputKeyType::GamepadButton, GLFW_GAMEPAD_BUTTON_DPAD_DOWN } } },
    { "GAMEPAD_BUTTON_DPAD_LEFT",     InputActionType::Gamepad, { { InputKeyType::GamepadButton, GLFW_GAMEPAD_BUTTON_DPAD_LEFT } } },
    { "GAMEPAD_BUTTON_LAST",          InputActionType::Gamepad, { { InputKeyType::GamepadButton, GLFW_GAMEPAD_BUTTON_LAST } } },
    { "GAMEPAD_BUTTON_CROSS",         InputActionType::Gamepad, { { InputKeyType::GamepadButton, GLFW_GAMEPAD_BUTTON_CROSS } } },
    { "GAMEPAD_BUTTON_CIRCLE",        InputActionType::Gamepad, { { InputKeyType::GamepadButton, GLFW_GAMEPAD_BUTTON_CIRCLE } } },
    { "GAMEPAD_BUTTON_SQUARE",        InputActionType::Gamepad, { { InputKeyType::GamepadButton, GLFW_GAMEPAD_BUTTON_SQUARE } } },
    { "GAMEPAD_BUTTON_TRIANGLE",      InputActionType::Gamepad, { { InputKeyType::GamepadButton, GLFW_GAMEPAD_BUTTON_TRIANGLE } } },
    { "GAMEPAD_AXIS_LEFT_X",          InputActionType::Gamepad, { { InputKeyType::GamepadAxis, GLFW_GAMEPAD_AXIS_LEFT_X } } },
    { "GAMEPAD_AXIS_LEFT_Y",          InputActionType::Gamepad, { { InputKeyType::GamepadAxis, GLFW_GAMEPAD_AXIS_LEFT_Y } } },
    { "GAMEPAD_AXIS_RIGHT_X",         InputActionType::Gamepad, { { InputKeyType::GamepadAxis, GLFW_GAMEPAD_AXIS_RIGHT_X } } },
    { "GAMEPAD_AXIS_RIGHT_Y",         InputActionType::Gamepad, { { InputKeyType::GamepadAxis, GLFW_GAMEPAD_AXIS_RIGHT_Y } } },
    { "GAMEPAD_AXIS_LEFT_TRIGGER",    InputActionType::Gamepad, { { InputKeyType::GamepadAxis, GLFW_GAMEPAD_AXIS_LEFT_TRIGGER } } },
    { "GAMEPAD_AXIS_RIGHT_TRIGGER",   InputActionType::Gamepad, { { InputKeyType::GamepadAxis, GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER } } },
    { "GAMEPAD_AXIS_LAST",            InputActionType::Gamepad, { { InputKeyType::GamepadAxis, GLFW_GAMEPAD_AXIS_LAST } } },
  };
}

void Input::joystick_callback(int jid, int event)
{
  if(event == GLFW_CONNECTED)
  {
    Input::connectedJoysticks.push_back(jid);
    LOGGER_DEBUG("Joystick {} connected", jid);
  }
  else if(event == GLFW_DISCONNECTED)
  {
    LOGGER_DEBUG("Joystick {} disconnected", jid);
    for(size_t i = 0; i < Input::connectedJoysticks.size(); i++)
    {
      auto connectedJid = Input::connectedJoysticks[i];

      if(connectedJid == jid)
      {
        Input::connectedJoysticks.erase(Input::connectedJoysticks.begin() + i);
        break;
      }
    }
  }
}

void Input::addConnectedJoysticks()
{
  int maxJoysticks = 16;

  for(size_t i = 0; i < maxJoysticks; i++)
  {
    int state = glfwJoystickPresent(GLFW_JOYSTICK_1 + i);

    if(state == 1)
    {
      LOGGER_DEBUG("Joystick {} connected", GLFW_JOYSTICK_1 + i);
      Input::connectedJoysticks.push_back(GLFW_JOYSTICK_1 + i);
    }
  }
}