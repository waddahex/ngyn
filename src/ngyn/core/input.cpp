#include "input.hpp"

using namespace ngyn;

ngyn::Input::Input(InputCreateInfo createInfo)
{
  if(createInfo.usePrintableKeys)
  {
    auto printableKeysMap = printableKeys();
    this->actions.insert(printableKeysMap.begin(), printableKeysMap.end());
  }

  if(createInfo.useFunctionalKeys)
  {
    auto functionalKeysMap = functionalKeys();
    this->actions.insert(functionalKeysMap.begin(), functionalKeysMap.end());
  }

  if(createInfo.useMouseButtons)
  {
    auto mouseButtonsMap = mouseButtons();
    this->actions.insert(mouseButtonsMap.begin(), mouseButtonsMap.end());
  }

  if(createInfo.useJoystickButtons)
  {
    auto joystickButtonsMap = joystickButtons();
    this->actions.insert(joystickButtonsMap.begin(), joystickButtonsMap.end());
  }

  if(createInfo.useGamepadButtons)
  {
    auto gamepadButtonsMap = gamepadButtons();
    this->actions.insert(gamepadButtonsMap.begin(), gamepadButtonsMap.end());
  }
}

void ngyn::Input::update(const Window &window)
{
  for(auto &[_, action] : this->actions)
  {
    action.state.released = false;

    for(size_t i = 0; i < action.keys.size(); i++)
    {
      auto key = action.keys[i];

      int state = key.origin == InputOrigin::Keyboard ?
        glfwGetKey(window.handle, key.value) :
        glfwGetMouseButton(window.handle, key.value);

      if(state == GLFW_PRESS)
      {
        if(!action.state.pressed && !action.state.held)
        {
          action.state.released = false;
          action.state.pressed = true;
          action.state.held = true;
        }
        else
        {
          action.state.released = false;
          action.state.pressed = false;
          action.state.held = true;
        }
      }
      else if(state == GLFW_RELEASE)
      {
        // Consider only the state of the last key of the action
        if(i == action.keys.size() -1)
        {
          // Only can be InputAction.relased if it was held or pressed
          if(action.state.pressed || action.state.held)
          {
            action.state.released = true;
          }

          action.state.held = false;
          action.state.pressed = false;
        }
      }
    }
  }
}

bool ngyn::Input::held(const std::string &actionName)
{
  auto action = this->actions[actionName];
  return action.state.held;
}

bool ngyn::Input::pressed(const std::string &actionName)
{
  auto action = this->actions[actionName];
  return action.state.pressed;
}

bool ngyn::Input::released(const std::string &actionName)
{
  auto action = this->actions[actionName];
  return action.state.released;
}

std::unordered_map<std::string, InputAction> ngyn::Input::printableKeys()
{
  return {
    { "KEY_SPACE",         { {}, { { InputOrigin::Keyboard, GLFW_KEY_SPACE } } } },
    { "KEY_APOSTROPHE",    { {}, { { InputOrigin::Keyboard, GLFW_KEY_APOSTROPHE } } } },
    { "KEY_COMMA",         { {}, { { InputOrigin::Keyboard, GLFW_KEY_COMMA } } } },
    { "KEY_MINUS",         { {}, { { InputOrigin::Keyboard, GLFW_KEY_MINUS } } } },
    { "KEY_PERIOD",        { {}, { { InputOrigin::Keyboard, GLFW_KEY_PERIOD } } } },
    { "KEY_SLASH",         { {}, { { InputOrigin::Keyboard, GLFW_KEY_SLASH } } } },
    { "KEY_0",             { {}, { { InputOrigin::Keyboard, GLFW_KEY_0 } } } },
    { "KEY_1",             { {}, { { InputOrigin::Keyboard, GLFW_KEY_1 } } } },
    { "KEY_2",             { {}, { { InputOrigin::Keyboard, GLFW_KEY_2 } } } },
    { "KEY_3",             { {}, { { InputOrigin::Keyboard, GLFW_KEY_3 } } } },
    { "KEY_4",             { {}, { { InputOrigin::Keyboard, GLFW_KEY_4 } } } },
    { "KEY_5",             { {}, { { InputOrigin::Keyboard, GLFW_KEY_5 } } } },
    { "KEY_6",             { {}, { { InputOrigin::Keyboard, GLFW_KEY_6 } } } },
    { "KEY_7",             { {}, { { InputOrigin::Keyboard, GLFW_KEY_7 } } } },
    { "KEY_8",             { {}, { { InputOrigin::Keyboard, GLFW_KEY_8 } } } },
    { "KEY_9",             { {}, { { InputOrigin::Keyboard, GLFW_KEY_9 } } } },
    { "KEY_SEMICOLON",     { {}, { { InputOrigin::Keyboard, GLFW_KEY_SEMICOLON } } } },
    { "KEY_EQUAL",         { {}, { { InputOrigin::Keyboard, GLFW_KEY_EQUAL } } } },
    { "KEY_A",             { {}, { { InputOrigin::Keyboard, GLFW_KEY_A } } } },
    { "KEY_B",             { {}, { { InputOrigin::Keyboard, GLFW_KEY_B } } } },
    { "KEY_C",             { {}, { { InputOrigin::Keyboard, GLFW_KEY_C } } } },
    { "KEY_D",             { {}, { { InputOrigin::Keyboard, GLFW_KEY_D } } } },
    { "KEY_E",             { {}, { { InputOrigin::Keyboard, GLFW_KEY_E } } } },
    { "KEY_F",             { {}, { { InputOrigin::Keyboard, GLFW_KEY_F } } } },
    { "KEY_G",             { {}, { { InputOrigin::Keyboard, GLFW_KEY_G } } } },
    { "KEY_H",             { {}, { { InputOrigin::Keyboard, GLFW_KEY_H } } } },
    { "KEY_I",             { {}, { { InputOrigin::Keyboard, GLFW_KEY_I } } } },
    { "KEY_J",             { {}, { { InputOrigin::Keyboard, GLFW_KEY_J } } } },
    { "KEY_K",             { {}, { { InputOrigin::Keyboard, GLFW_KEY_K } } } },
    { "KEY_L",             { {}, { { InputOrigin::Keyboard, GLFW_KEY_L } } } },
    { "KEY_M",             { {}, { { InputOrigin::Keyboard, GLFW_KEY_M } } } },
    { "KEY_N",             { {}, { { InputOrigin::Keyboard, GLFW_KEY_N } } } },
    { "KEY_O",             { {}, { { InputOrigin::Keyboard, GLFW_KEY_O } } } },
    { "KEY_P",             { {}, { { InputOrigin::Keyboard, GLFW_KEY_P } } } },
    { "KEY_Q",             { {}, { { InputOrigin::Keyboard, GLFW_KEY_Q } } } },
    { "KEY_R",             { {}, { { InputOrigin::Keyboard, GLFW_KEY_R } } } },
    { "KEY_S",             { {}, { { InputOrigin::Keyboard, GLFW_KEY_S } } } },
    { "KEY_T",             { {}, { { InputOrigin::Keyboard, GLFW_KEY_T } } } },
    { "KEY_U",             { {}, { { InputOrigin::Keyboard, GLFW_KEY_U } } } },
    { "KEY_V",             { {}, { { InputOrigin::Keyboard, GLFW_KEY_V } } } },
    { "KEY_W",             { {}, { { InputOrigin::Keyboard, GLFW_KEY_W } } } },
    { "KEY_X",             { {}, { { InputOrigin::Keyboard, GLFW_KEY_X } } } },
    { "KEY_Y",             { {}, { { InputOrigin::Keyboard, GLFW_KEY_Y } } } },
    { "KEY_Z",             { {}, { { InputOrigin::Keyboard, GLFW_KEY_Z } } } },
    { "KEY_LEFT_BRACKET",  { {}, { { InputOrigin::Keyboard, GLFW_KEY_LEFT_BRACKET } } } },
    { "KEY_BACKSLASH",     { {}, { { InputOrigin::Keyboard, GLFW_KEY_BACKSLASH } } } },
    { "KEY_RIGHT_BRACKET", { {}, { { InputOrigin::Keyboard, GLFW_KEY_RIGHT_BRACKET } } } },
    { "KEY_GRAVE_ACCENT",  { {}, { { InputOrigin::Keyboard, GLFW_KEY_GRAVE_ACCENT } } } },
    { "KEY_WORLD_1",       { {}, { { InputOrigin::Keyboard, GLFW_KEY_WORLD_1 } } } },
    { "KEY_WORLD_2",       { {}, { { InputOrigin::Keyboard, GLFW_KEY_WORLD_2 } } } },
  };
}

std::unordered_map<std::string, InputAction> ngyn::Input::functionalKeys()
{
  return {
    { "KEY_ESCAPE",         { {}, { { InputOrigin::Keyboard, GLFW_KEY_ESCAPE } } } },
    { "KEY_ENTER",          { {}, { { InputOrigin::Keyboard, GLFW_KEY_ENTER } } } },
    { "KEY_TAB",            { {}, { { InputOrigin::Keyboard, GLFW_KEY_TAB } } } },
    { "KEY_BACKSPACE",      { {}, { { InputOrigin::Keyboard, GLFW_KEY_BACKSPACE } } } },
    { "KEY_INSERT",         { {}, { { InputOrigin::Keyboard, GLFW_KEY_INSERT } } } },
    { "KEY_DELETE",         { {}, { { InputOrigin::Keyboard, GLFW_KEY_DELETE } } } },
    { "KEY_RIGHT",          { {}, { { InputOrigin::Keyboard, GLFW_KEY_RIGHT } } } },
    { "KEY_LEFT",           { {}, { { InputOrigin::Keyboard, GLFW_KEY_LEFT } } } },
    { "KEY_DOWN",           { {}, { { InputOrigin::Keyboard, GLFW_KEY_DOWN } } } },
    { "KEY_UP",             { {}, { { InputOrigin::Keyboard, GLFW_KEY_UP } } } },
    { "KEY_PAGE_UP",        { {}, { { InputOrigin::Keyboard, GLFW_KEY_PAGE_UP } } } },
    { "KEY_PAGE_DOWN",      { {}, { { InputOrigin::Keyboard, GLFW_KEY_PAGE_DOWN } } } },
    { "KEY_HOME",           { {}, { { InputOrigin::Keyboard, GLFW_KEY_HOME } } } },
    { "KEY_END",            { {}, { { InputOrigin::Keyboard, GLFW_KEY_END } } } },
    { "KEY_CAPS_LOCK",      { {}, { { InputOrigin::Keyboard, GLFW_KEY_CAPS_LOCK } } } },
    { "KEY_SCROLL_LOCK",    { {}, { { InputOrigin::Keyboard, GLFW_KEY_SCROLL_LOCK } } } },
    { "KEY_NUM_LOCK",       { {}, { { InputOrigin::Keyboard, GLFW_KEY_NUM_LOCK } } } },
    { "KEY_PRINT_SCREEN",   { {}, { { InputOrigin::Keyboard, GLFW_KEY_PRINT_SCREEN } } } },
    { "KEY_PAUSE",          { {}, { { InputOrigin::Keyboard, GLFW_KEY_PAUSE } } } },
    { "KEY_F1",             { {}, { { InputOrigin::Keyboard, GLFW_KEY_F1 } } } },
    { "KEY_F2",             { {}, { { InputOrigin::Keyboard, GLFW_KEY_F2 } } } },
    { "KEY_F3",             { {}, { { InputOrigin::Keyboard, GLFW_KEY_F3 } } } },
    { "KEY_F4",             { {}, { { InputOrigin::Keyboard, GLFW_KEY_F4 } } } },
    { "KEY_F5",             { {}, { { InputOrigin::Keyboard, GLFW_KEY_F5 } } } },
    { "KEY_F6",             { {}, { { InputOrigin::Keyboard, GLFW_KEY_F6 } } } },
    { "KEY_F7",             { {}, { { InputOrigin::Keyboard, GLFW_KEY_F7 } } } },
    { "KEY_F8",             { {}, { { InputOrigin::Keyboard, GLFW_KEY_F8 } } } },
    { "KEY_F9",             { {}, { { InputOrigin::Keyboard, GLFW_KEY_F9 } } } },
    { "KEY_F10",            { {}, { { InputOrigin::Keyboard, GLFW_KEY_F10 } } } },
    { "KEY_F11",            { {}, { { InputOrigin::Keyboard, GLFW_KEY_F11 } } } },
    { "KEY_F12",            { {}, { { InputOrigin::Keyboard, GLFW_KEY_F12 } } } },
    { "KEY_F13",            { {}, { { InputOrigin::Keyboard, GLFW_KEY_F13 } } } },
    { "KEY_F14",            { {}, { { InputOrigin::Keyboard, GLFW_KEY_F14 } } } },
    { "KEY_F15",            { {}, { { InputOrigin::Keyboard, GLFW_KEY_F15 } } } },
    { "KEY_F16",            { {}, { { InputOrigin::Keyboard, GLFW_KEY_F16 } } } },
    { "KEY_F17",            { {}, { { InputOrigin::Keyboard, GLFW_KEY_F17 } } } },
    { "KEY_F18",            { {}, { { InputOrigin::Keyboard, GLFW_KEY_F18 } } } },
    { "KEY_F19",            { {}, { { InputOrigin::Keyboard, GLFW_KEY_F19 } } } },
    { "KEY_F20",            { {}, { { InputOrigin::Keyboard, GLFW_KEY_F20 } } } },
    { "KEY_F21",            { {}, { { InputOrigin::Keyboard, GLFW_KEY_F21 } } } },
    { "KEY_F22",            { {}, { { InputOrigin::Keyboard, GLFW_KEY_F22 } } } },
    { "KEY_F23",            { {}, { { InputOrigin::Keyboard, GLFW_KEY_F23 } } } },
    { "KEY_F24",            { {}, { { InputOrigin::Keyboard, GLFW_KEY_F24 } } } },
    { "KEY_F25",            { {}, { { InputOrigin::Keyboard, GLFW_KEY_F25 } } } },
    { "KEY_KP_0",           { {}, { { InputOrigin::Keyboard, GLFW_KEY_KP_0 } } } },
    { "KEY_KP_1",           { {}, { { InputOrigin::Keyboard, GLFW_KEY_KP_1 } } } },
    { "KEY_KP_2",           { {}, { { InputOrigin::Keyboard, GLFW_KEY_KP_2 } } } },
    { "KEY_KP_3",           { {}, { { InputOrigin::Keyboard, GLFW_KEY_KP_3 } } } },
    { "KEY_KP_4",           { {}, { { InputOrigin::Keyboard, GLFW_KEY_KP_4 } } } },
    { "KEY_KP_5",           { {}, { { InputOrigin::Keyboard, GLFW_KEY_KP_5 } } } },
    { "KEY_KP_6",           { {}, { { InputOrigin::Keyboard, GLFW_KEY_KP_6 } } } },
    { "KEY_KP_7",           { {}, { { InputOrigin::Keyboard, GLFW_KEY_KP_7 } } } },
    { "KEY_KP_8",           { {}, { { InputOrigin::Keyboard, GLFW_KEY_KP_8 } } } },
    { "KEY_KP_9",           { {}, { { InputOrigin::Keyboard, GLFW_KEY_KP_9 } } } },
    { "KEY_KP_DECIMAL",     { {}, { { InputOrigin::Keyboard, GLFW_KEY_KP_DECIMAL } } } },
    { "KEY_KP_DIVIDE",      { {}, { { InputOrigin::Keyboard, GLFW_KEY_KP_DIVIDE } } } },
    { "KEY_KP_MULTIPLY",    { {}, { { InputOrigin::Keyboard, GLFW_KEY_KP_MULTIPLY } } } },
    { "KEY_KP_SUBTRACT",    { {}, { { InputOrigin::Keyboard, GLFW_KEY_KP_SUBTRACT } } } },
    { "KEY_KP_ADD",         { {}, { { InputOrigin::Keyboard, GLFW_KEY_KP_ADD } } } },
    { "KEY_KP_ENTER",       { {}, { { InputOrigin::Keyboard, GLFW_KEY_KP_ENTER } } } },
    { "KEY_KP_EQUAL",       { {}, { { InputOrigin::Keyboard, GLFW_KEY_KP_EQUAL } } } },
    { "KEY_LEFT_SHIFT",     { {}, { { InputOrigin::Keyboard, GLFW_KEY_LEFT_SHIFT } } } },
    { "KEY_LEFT_CONTROL",   { {}, { { InputOrigin::Keyboard, GLFW_KEY_LEFT_CONTROL } } } },
    { "KEY_LEFT_ALT",       { {}, { { InputOrigin::Keyboard, GLFW_KEY_LEFT_ALT } } } },
    { "KEY_LEFT_SUPER",     { {}, { { InputOrigin::Keyboard, GLFW_KEY_LEFT_SUPER } } } },
    { "KEY_RIGHT_SHIFT",    { {}, { { InputOrigin::Keyboard, GLFW_KEY_RIGHT_SHIFT } } } },
    { "KEY_RIGHT_CONTROL",  { {}, { { InputOrigin::Keyboard, GLFW_KEY_RIGHT_CONTROL } } } },
    { "KEY_RIGHT_ALT",      { {}, { { InputOrigin::Keyboard, GLFW_KEY_RIGHT_ALT } } } },
    { "KEY_RIGHT_SUPER",    { {}, { { InputOrigin::Keyboard, GLFW_KEY_RIGHT_SUPER } } } },
    { "KEY_MENU",           { {}, { { InputOrigin::Keyboard, GLFW_KEY_MENU } } } },
    { "KEY_LAST",           { {}, { { InputOrigin::Keyboard, GLFW_KEY_LAST } } } },
  };
}

std::unordered_map<std::string, InputAction> ngyn::Input::mouseButtons()
{
  return {
    { "MOUSE_BUTTON_1",       { {}, { { InputOrigin::Mouse, GLFW_MOUSE_BUTTON_1 } } } },
    { "MOUSE_BUTTON_2",       { {}, { { InputOrigin::Mouse, GLFW_MOUSE_BUTTON_2 } } } },
    { "MOUSE_BUTTON_3",       { {}, { { InputOrigin::Mouse, GLFW_MOUSE_BUTTON_3 } } } },
    { "MOUSE_BUTTON_4",       { {}, { { InputOrigin::Mouse, GLFW_MOUSE_BUTTON_4 } } } },
    { "MOUSE_BUTTON_5",       { {}, { { InputOrigin::Mouse, GLFW_MOUSE_BUTTON_5 } } } },
    { "MOUSE_BUTTON_6",       { {}, { { InputOrigin::Mouse, GLFW_MOUSE_BUTTON_6 } } } },
    { "MOUSE_BUTTON_7",       { {}, { { InputOrigin::Mouse, GLFW_MOUSE_BUTTON_7 } } } },
    { "MOUSE_BUTTON_8",       { {}, { { InputOrigin::Mouse, GLFW_MOUSE_BUTTON_8 } } } },
    { "MOUSE_BUTTON_LAST",    { {}, { { InputOrigin::Mouse, GLFW_MOUSE_BUTTON_LAST } } } },
    { "MOUSE_BUTTON_LEFT",    { {}, { { InputOrigin::Mouse, GLFW_MOUSE_BUTTON_LEFT } } } },
    { "MOUSE_BUTTON_RIGHT",   { {}, { { InputOrigin::Mouse, GLFW_MOUSE_BUTTON_RIGHT } } } },
    { "MOUSE_BUTTON_MIDDLE",  { {}, { { InputOrigin::Mouse, GLFW_MOUSE_BUTTON_MIDDLE } } } },
  };
}

std::unordered_map<std::string, InputAction> ngyn::Input::joystickButtons()
{
  return {
    { "JOYSTICK_1",    { {}, { { InputOrigin::Joystick, GLFW_JOYSTICK_1 } } } },
    { "JOYSTICK_2",    { {}, { { InputOrigin::Joystick, GLFW_JOYSTICK_2 } } } },
    { "JOYSTICK_3",    { {}, { { InputOrigin::Joystick, GLFW_JOYSTICK_3 } } } },
    { "JOYSTICK_4",    { {}, { { InputOrigin::Joystick, GLFW_JOYSTICK_4 } } } },
    { "JOYSTICK_5",    { {}, { { InputOrigin::Joystick, GLFW_JOYSTICK_5 } } } },
    { "JOYSTICK_6",    { {}, { { InputOrigin::Joystick, GLFW_JOYSTICK_6 } } } },
    { "JOYSTICK_7",    { {}, { { InputOrigin::Joystick, GLFW_JOYSTICK_7 } } } },
    { "JOYSTICK_8",    { {}, { { InputOrigin::Joystick, GLFW_JOYSTICK_8 } } } },
    { "JOYSTICK_9",    { {}, { { InputOrigin::Joystick, GLFW_JOYSTICK_9 } } } },
    { "JOYSTICK_10",   { {}, { { InputOrigin::Joystick, GLFW_JOYSTICK_10 } } } },
    { "JOYSTICK_11",   { {}, { { InputOrigin::Joystick, GLFW_JOYSTICK_11 } } } },
    { "JOYSTICK_12",   { {}, { { InputOrigin::Joystick, GLFW_JOYSTICK_12 } } } },
    { "JOYSTICK_13",   { {}, { { InputOrigin::Joystick, GLFW_JOYSTICK_13 } } } },
    { "JOYSTICK_14",   { {}, { { InputOrigin::Joystick, GLFW_JOYSTICK_14 } } } },
    { "JOYSTICK_15",   { {}, { { InputOrigin::Joystick, GLFW_JOYSTICK_15 } } } },
    { "JOYSTICK_16",   { {}, { { InputOrigin::Joystick, GLFW_JOYSTICK_16 } } } },
    { "JOYSTICK_LAST", { {}, { { InputOrigin::Joystick, GLFW_JOYSTICK_LAST } } } },
  };
}

std::unordered_map<std::string, InputAction> ngyn::Input::gamepadButtons()
{
  return {
    { "GAMEPAD_BUTTON_A",             { {}, { { InputOrigin::Gamepad, GLFW_GAMEPAD_BUTTON_A } } } },
    { "GAMEPAD_BUTTON_B",             { {}, { { InputOrigin::Gamepad, GLFW_GAMEPAD_BUTTON_B } } } },
    { "GAMEPAD_BUTTON_X",             { {}, { { InputOrigin::Gamepad, GLFW_GAMEPAD_BUTTON_X } } } },
    { "GAMEPAD_BUTTON_Y",             { {}, { { InputOrigin::Gamepad, GLFW_GAMEPAD_BUTTON_Y } } } },
    { "GAMEPAD_BUTTON_LEFT_BUMPER",   { {}, { { InputOrigin::Gamepad, GLFW_GAMEPAD_BUTTON_LEFT_BUMPER } } } },
    { "GAMEPAD_BUTTON_RIGHT_BUMPER",  { {}, { { InputOrigin::Gamepad, GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER } } } },
    { "GAMEPAD_BUTTON_BACK",          { {}, { { InputOrigin::Gamepad, GLFW_GAMEPAD_BUTTON_BACK } } } },
    { "GAMEPAD_BUTTON_START",         { {}, { { InputOrigin::Gamepad, GLFW_GAMEPAD_BUTTON_START } } } },
    { "GAMEPAD_BUTTON_GUIDE",         { {}, { { InputOrigin::Gamepad, GLFW_GAMEPAD_BUTTON_GUIDE } } } },
    { "GAMEPAD_BUTTON_LEFT_THUMB",    { {}, { { InputOrigin::Gamepad, GLFW_GAMEPAD_BUTTON_LEFT_THUMB } } } },
    { "GAMEPAD_BUTTON_RIGHT_THUMB",   { {}, { { InputOrigin::Gamepad, GLFW_GAMEPAD_BUTTON_RIGHT_THUMB } } } },
    { "GAMEPAD_BUTTON_DPAD_UP",       { {}, { { InputOrigin::Gamepad, GLFW_GAMEPAD_BUTTON_DPAD_UP } } } },
    { "GAMEPAD_BUTTON_DPAD_RIGHT",    { {}, { { InputOrigin::Gamepad, GLFW_GAMEPAD_BUTTON_DPAD_RIGHT } } } },
    { "GAMEPAD_BUTTON_DPAD_DOWN",     { {}, { { InputOrigin::Gamepad, GLFW_GAMEPAD_BUTTON_DPAD_DOWN } } } },
    { "GAMEPAD_BUTTON_DPAD_LEFT",     { {}, { { InputOrigin::Gamepad, GLFW_GAMEPAD_BUTTON_DPAD_LEFT } } } },
    { "GAMEPAD_BUTTON_LAST",          { {}, { { InputOrigin::Gamepad, GLFW_GAMEPAD_BUTTON_LAST } } } },
    { "GAMEPAD_BUTTON_CROSS",         { {}, { { InputOrigin::Gamepad, GLFW_GAMEPAD_BUTTON_CROSS } } } },
    { "GAMEPAD_BUTTON_CIRCLE",        { {}, { { InputOrigin::Gamepad, GLFW_GAMEPAD_BUTTON_CIRCLE } } } },
    { "GAMEPAD_BUTTON_SQUARE",        { {}, { { InputOrigin::Gamepad, GLFW_GAMEPAD_BUTTON_SQUARE } } } },
    { "GAMEPAD_BUTTON_TRIANGLE",      { {}, { { InputOrigin::Gamepad, GLFW_GAMEPAD_BUTTON_TRIANGLE } } } },
    { "GAMEPAD_AXIS_LEFT_X",          { {}, { { InputOrigin::Gamepad, GLFW_GAMEPAD_AXIS_LEFT_X } } } },
    { "GAMEPAD_AXIS_LEFT_Y",          { {}, { { InputOrigin::Gamepad, GLFW_GAMEPAD_AXIS_LEFT_Y } } } },
    { "GAMEPAD_AXIS_RIGHT_X",         { {}, { { InputOrigin::Gamepad, GLFW_GAMEPAD_AXIS_RIGHT_X } } } },
    { "GAMEPAD_AXIS_RIGHT_Y",         { {}, { { InputOrigin::Gamepad, GLFW_GAMEPAD_AXIS_RIGHT_Y } } } },
    { "GAMEPAD_AXIS_LEFT_TRIGGER",    { {}, { { InputOrigin::Gamepad, GLFW_GAMEPAD_AXIS_LEFT_TRIGGER } } } },
    { "GAMEPAD_AXIS_RIGHT_TRIGGER",   { {}, { { InputOrigin::Gamepad, GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER } } } },
    { "GAMEPAD_AXIS_LAST",            { {}, { { InputOrigin::Gamepad, GLFW_GAMEPAD_AXIS_LAST } } } },
  };
}