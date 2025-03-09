#include "input.hpp"

using namespace ngyn;

std::vector<int> Input::connectedJoysticks;

Input::Input(CreateInfo createInfo)
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
    if(action.type == ActionType::Gamepad)
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

        auto keyState = key.origin == KeyType::GamepadButton ?
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

    int keyState = key.origin == KeyType::Keyboard ?
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

int Input::intensity(const std::string &actionName)
{
  if(this->states.find(actionName) != this->states.end())
  {
    return this->states[actionName].intensity;
  }
  return 0;
}

Input::State Input::state(const std::string &actionName)
{
  if(this->states.find(actionName) != this->states.end())
  {
    return this->states[actionName];
  }
  return State();
}

bool Input::hasJoystick(int index)
{
  return std::find(connectedJoysticks.begin(), connectedJoysticks.end(), index) != connectedJoysticks.end();
}

std::vector<Input::Action> Input::printableKeys()
{
  return {
    { "KEY_SPACE",         ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_SPACE } } },
    { "KEY_APOSTROPHE",    ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_APOSTROPHE } } },
    { "KEY_COMMA",         ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_COMMA } } },
    { "KEY_MINUS",         ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_MINUS } } },
    { "KEY_PERIOD",        ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_PERIOD } } },
    { "KEY_SLASH",         ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_SLASH } } },
    { "KEY_0",             ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_0 } } },
    { "KEY_1",             ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_1 } } },
    { "KEY_2",             ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_2 } } },
    { "KEY_3",             ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_3 } } },
    { "KEY_4",             ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_4 } } },
    { "KEY_5",             ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_5 } } },
    { "KEY_6",             ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_6 } } },
    { "KEY_7",             ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_7 } } },
    { "KEY_8",             ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_8 } } },
    { "KEY_9",             ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_9 } } },
    { "KEY_SEMICOLON",     ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_SEMICOLON } } },
    { "KEY_EQUAL",         ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_EQUAL } } },
    { "KEY_A",             ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_A } } },
    { "KEY_B",             ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_B } } },
    { "KEY_C",             ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_C } } },
    { "KEY_D",             ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_D } } },
    { "KEY_E",             ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_E } } },
    { "KEY_F",             ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_F } } },
    { "KEY_G",             ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_G } } },
    { "KEY_H",             ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_H } } },
    { "KEY_I",             ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_I } } },
    { "KEY_J",             ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_J } } },
    { "KEY_K",             ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_K } } },
    { "KEY_L",             ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_L } } },
    { "KEY_M",             ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_M } } },
    { "KEY_N",             ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_N } } },
    { "KEY_O",             ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_O } } },
    { "KEY_P",             ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_P } } },
    { "KEY_Q",             ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_Q } } },
    { "KEY_R",             ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_R } } },
    { "KEY_S",             ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_S } } },
    { "KEY_T",             ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_T } } },
    { "KEY_U",             ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_U } } },
    { "KEY_V",             ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_V } } },
    { "KEY_W",             ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_W } } },
    { "KEY_X",             ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_X } } },
    { "KEY_Y",             ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_Y } } },
    { "KEY_Z",             ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_Z } } },
    { "KEY_LEFT_BRACKET",  ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_LEFT_BRACKET } } },
    { "KEY_BACKSLASH",     ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_BACKSLASH } } },
    { "KEY_RIGHT_BRACKET", ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_RIGHT_BRACKET } } },
    { "KEY_GRAVE_ACCENT",  ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_GRAVE_ACCENT } } },
    { "KEY_WORLD_1",       ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_WORLD_1 } } },
    { "KEY_WORLD_2",       ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_WORLD_2 } } },
  };
}

std::vector<Input::Action> Input::functionalKeys()
{
  return {
    { "KEY_ESCAPE",         ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_ESCAPE } } },
    { "KEY_ENTER",          ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_ENTER } } },
    { "KEY_TAB",            ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_TAB } } },
    { "KEY_BACKSPACE",      ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_BACKSPACE } } },
    { "KEY_INSERT",         ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_INSERT } } },
    { "KEY_DELETE",         ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_DELETE } } },
    { "KEY_RIGHT",          ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_RIGHT } } },
    { "KEY_LEFT",           ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_LEFT } } },
    { "KEY_DOWN",           ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_DOWN } } },
    { "KEY_UP",             ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_UP } } },
    { "KEY_PAGE_UP",        ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_PAGE_UP } } },
    { "KEY_PAGE_DOWN",      ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_PAGE_DOWN } } },
    { "KEY_HOME",           ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_HOME } } },
    { "KEY_END",            ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_END } } },
    { "KEY_CAPS_LOCK",      ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_CAPS_LOCK } } },
    { "KEY_SCROLL_LOCK",    ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_SCROLL_LOCK } } },
    { "KEY_NUM_LOCK",       ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_NUM_LOCK } } },
    { "KEY_PRINT_SCREEN",   ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_PRINT_SCREEN } } },
    { "KEY_PAUSE",          ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_PAUSE } } },
    { "KEY_F1",             ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_F1 } } },
    { "KEY_F2",             ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_F2 } } },
    { "KEY_F3",             ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_F3 } } },
    { "KEY_F4",             ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_F4 } } },
    { "KEY_F5",             ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_F5 } } },
    { "KEY_F6",             ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_F6 } } },
    { "KEY_F7",             ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_F7 } } },
    { "KEY_F8",             ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_F8 } } },
    { "KEY_F9",             ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_F9 } } },
    { "KEY_F10",            ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_F10 } } },
    { "KEY_F11",            ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_F11 } } },
    { "KEY_F12",            ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_F12 } } },
    { "KEY_F13",            ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_F13 } } },
    { "KEY_F14",            ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_F14 } } },
    { "KEY_F15",            ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_F15 } } },
    { "KEY_F16",            ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_F16 } } },
    { "KEY_F17",            ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_F17 } } },
    { "KEY_F18",            ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_F18 } } },
    { "KEY_F19",            ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_F19 } } },
    { "KEY_F20",            ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_F20 } } },
    { "KEY_F21",            ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_F21 } } },
    { "KEY_F22",            ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_F22 } } },
    { "KEY_F23",            ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_F23 } } },
    { "KEY_F24",            ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_F24 } } },
    { "KEY_F25",            ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_F25 } } },
    { "KEY_KP_0",           ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_KP_0 } } },
    { "KEY_KP_1",           ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_KP_1 } } },
    { "KEY_KP_2",           ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_KP_2 } } },
    { "KEY_KP_3",           ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_KP_3 } } },
    { "KEY_KP_4",           ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_KP_4 } } },
    { "KEY_KP_5",           ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_KP_5 } } },
    { "KEY_KP_6",           ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_KP_6 } } },
    { "KEY_KP_7",           ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_KP_7 } } },
    { "KEY_KP_8",           ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_KP_8 } } },
    { "KEY_KP_9",           ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_KP_9 } } },
    { "KEY_KP_DECIMAL",     ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_KP_DECIMAL } } },
    { "KEY_KP_DIVIDE",      ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_KP_DIVIDE } } },
    { "KEY_KP_MULTIPLY",    ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_KP_MULTIPLY } } },
    { "KEY_KP_SUBTRACT",    ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_KP_SUBTRACT } } },
    { "KEY_KP_ADD",         ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_KP_ADD } } },
    { "KEY_KP_ENTER",       ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_KP_ENTER } } },
    { "KEY_KP_EQUAL",       ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_KP_EQUAL } } },
    { "KEY_LEFT_SHIFT",     ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_LEFT_SHIFT } } },
    { "KEY_LEFT_CONTROL",   ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_LEFT_CONTROL } } },
    { "KEY_LEFT_ALT",       ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_LEFT_ALT } } },
    { "KEY_LEFT_SUPER",     ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_LEFT_SUPER } } },
    { "KEY_RIGHT_SHIFT",    ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_RIGHT_SHIFT } } },
    { "KEY_RIGHT_CONTROL",  ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_RIGHT_CONTROL } } },
    { "KEY_RIGHT_ALT",      ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_RIGHT_ALT } } },
    { "KEY_RIGHT_SUPER",    ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_RIGHT_SUPER } } },
    { "KEY_MENU",           ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_MENU } } },
    { "KEY_LAST",           ActionType::Keyboard, { { KeyType::Keyboard, GLFW_KEY_LAST } } },
  };
}

std::vector<Input::Action> Input::mouseButtons()
{
  return {
    { "MOUSE_BUTTON_1",       ActionType::Mouse, { { KeyType::Mouse, GLFW_MOUSE_BUTTON_1 } } },
    { "MOUSE_BUTTON_2",       ActionType::Mouse, { { KeyType::Mouse, GLFW_MOUSE_BUTTON_2 } } },
    { "MOUSE_BUTTON_3",       ActionType::Mouse, { { KeyType::Mouse, GLFW_MOUSE_BUTTON_3 } } },
    { "MOUSE_BUTTON_4",       ActionType::Mouse, { { KeyType::Mouse, GLFW_MOUSE_BUTTON_4 } } },
    { "MOUSE_BUTTON_5",       ActionType::Mouse, { { KeyType::Mouse, GLFW_MOUSE_BUTTON_5 } } },
    { "MOUSE_BUTTON_6",       ActionType::Mouse, { { KeyType::Mouse, GLFW_MOUSE_BUTTON_6 } } },
    { "MOUSE_BUTTON_7",       ActionType::Mouse, { { KeyType::Mouse, GLFW_MOUSE_BUTTON_7 } } },
    { "MOUSE_BUTTON_8",       ActionType::Mouse, { { KeyType::Mouse, GLFW_MOUSE_BUTTON_8 } } },
    { "MOUSE_BUTTON_LAST",    ActionType::Mouse, { { KeyType::Mouse, GLFW_MOUSE_BUTTON_LAST } } },
    { "MOUSE_BUTTON_LEFT",    ActionType::Mouse, { { KeyType::Mouse, GLFW_MOUSE_BUTTON_LEFT } } },
    { "MOUSE_BUTTON_RIGHT",   ActionType::Mouse, { { KeyType::Mouse, GLFW_MOUSE_BUTTON_RIGHT } } },
    { "MOUSE_BUTTON_MIDDLE",  ActionType::Mouse, { { KeyType::Mouse, GLFW_MOUSE_BUTTON_MIDDLE } } },
  };
}

std::vector<Input::Action> Input::gamepadButtons()
{
  return {
    { "GAMEPAD_BUTTON_A",             ActionType::Gamepad, { { KeyType::GamepadButton, GLFW_GAMEPAD_BUTTON_A } } },
    { "GAMEPAD_BUTTON_B",             ActionType::Gamepad, { { KeyType::GamepadButton, GLFW_GAMEPAD_BUTTON_B } } },
    { "GAMEPAD_BUTTON_X",             ActionType::Gamepad, { { KeyType::GamepadButton, GLFW_GAMEPAD_BUTTON_X } } },
    { "GAMEPAD_BUTTON_Y",             ActionType::Gamepad, { { KeyType::GamepadButton, GLFW_GAMEPAD_BUTTON_Y } } },
    { "GAMEPAD_BUTTON_LEFT_BUMPER",   ActionType::Gamepad, { { KeyType::GamepadButton, GLFW_GAMEPAD_BUTTON_LEFT_BUMPER } } },
    { "GAMEPAD_BUTTON_RIGHT_BUMPER",  ActionType::Gamepad, { { KeyType::GamepadButton, GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER } } },
    { "GAMEPAD_BUTTON_BACK",          ActionType::Gamepad, { { KeyType::GamepadButton, GLFW_GAMEPAD_BUTTON_BACK } } },
    { "GAMEPAD_BUTTON_START",         ActionType::Gamepad, { { KeyType::GamepadButton, GLFW_GAMEPAD_BUTTON_START } } },
    { "GAMEPAD_BUTTON_GUIDE",         ActionType::Gamepad, { { KeyType::GamepadButton, GLFW_GAMEPAD_BUTTON_GUIDE } } },
    { "GAMEPAD_BUTTON_LEFT_THUMB",    ActionType::Gamepad, { { KeyType::GamepadButton, GLFW_GAMEPAD_BUTTON_LEFT_THUMB } } },
    { "GAMEPAD_BUTTON_RIGHT_THUMB",   ActionType::Gamepad, { { KeyType::GamepadButton, GLFW_GAMEPAD_BUTTON_RIGHT_THUMB } } },
    { "GAMEPAD_BUTTON_DPAD_UP",       ActionType::Gamepad, { { KeyType::GamepadButton, GLFW_GAMEPAD_BUTTON_DPAD_UP } } },
    { "GAMEPAD_BUTTON_DPAD_RIGHT",    ActionType::Gamepad, { { KeyType::GamepadButton, GLFW_GAMEPAD_BUTTON_DPAD_RIGHT } } },
    { "GAMEPAD_BUTTON_DPAD_DOWN",     ActionType::Gamepad, { { KeyType::GamepadButton, GLFW_GAMEPAD_BUTTON_DPAD_DOWN } } },
    { "GAMEPAD_BUTTON_DPAD_LEFT",     ActionType::Gamepad, { { KeyType::GamepadButton, GLFW_GAMEPAD_BUTTON_DPAD_LEFT } } },
    { "GAMEPAD_BUTTON_LAST",          ActionType::Gamepad, { { KeyType::GamepadButton, GLFW_GAMEPAD_BUTTON_LAST } } },
    { "GAMEPAD_BUTTON_CROSS",         ActionType::Gamepad, { { KeyType::GamepadButton, GLFW_GAMEPAD_BUTTON_CROSS } } },
    { "GAMEPAD_BUTTON_CIRCLE",        ActionType::Gamepad, { { KeyType::GamepadButton, GLFW_GAMEPAD_BUTTON_CIRCLE } } },
    { "GAMEPAD_BUTTON_SQUARE",        ActionType::Gamepad, { { KeyType::GamepadButton, GLFW_GAMEPAD_BUTTON_SQUARE } } },
    { "GAMEPAD_BUTTON_TRIANGLE",      ActionType::Gamepad, { { KeyType::GamepadButton, GLFW_GAMEPAD_BUTTON_TRIANGLE } } },
    { "GAMEPAD_AXIS_LEFT_X",          ActionType::Gamepad, { { KeyType::GamepadAxis, GLFW_GAMEPAD_AXIS_LEFT_X } } },
    { "GAMEPAD_AXIS_LEFT_Y",          ActionType::Gamepad, { { KeyType::GamepadAxis, GLFW_GAMEPAD_AXIS_LEFT_Y } } },
    { "GAMEPAD_AXIS_RIGHT_X",         ActionType::Gamepad, { { KeyType::GamepadAxis, GLFW_GAMEPAD_AXIS_RIGHT_X } } },
    { "GAMEPAD_AXIS_RIGHT_Y",         ActionType::Gamepad, { { KeyType::GamepadAxis, GLFW_GAMEPAD_AXIS_RIGHT_Y } } },
    { "GAMEPAD_AXIS_LEFT_TRIGGER",    ActionType::Gamepad, { { KeyType::GamepadAxis, GLFW_GAMEPAD_AXIS_LEFT_TRIGGER } } },
    { "GAMEPAD_AXIS_RIGHT_TRIGGER",   ActionType::Gamepad, { { KeyType::GamepadAxis, GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER } } },
    { "GAMEPAD_AXIS_LAST",            ActionType::Gamepad, { { KeyType::GamepadAxis, GLFW_GAMEPAD_AXIS_LAST } } },
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