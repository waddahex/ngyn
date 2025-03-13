#pragma once

#include "../../ngynpch.hpp"

#include "../util/logger.hpp"

namespace ngyn
{
  class Input
  {
    public:
    struct State
    {
      bool held       = false;
      bool pressed    = false;
      bool released   = false;
      float intensity = 0.0f; // For gamepad axes
    };
  
    enum KeyType
    {
      Keyboard,
      Mouse,
      GamepadButton,
      GamepadAxis,
    };
  
    enum class ActionType
    {
      Keyboard,
      Mouse,
      Gamepad,
      Mixed
    };
  
    struct Key
    {
      KeyType origin;
      int value;
    };
  
    struct Action
    {
      std::string name;
      ActionType type;
      std::vector<Key> keys; // GLFW key codes
    };
  
    struct CreateInfo
    {
      bool useKeyboard  = true;
      bool useMouse     = true;
      bool useGamepads  = true;
    };

    public:
    Input(CreateInfo createInfo = CreateInfo{});

    void update(GLFWwindow *windowHandle);

    template<typename... Args>
    bool held(Args&&... args)
    {
      return checkState("held", std::forward<Args>(args)...);
    }

    template<typename... Args>
    bool pressed(Args&&... args)
    {
      return checkState("pressed", std::forward<Args>(args)...);
    }

    template<typename... Args>
    bool released(Args&&... args)
    {
      return checkState("released", std::forward<Args>(args)...);
    }

    int intensity(const std::string &actionName);
    State state(const std::string &actionName);

    bool hasJoystick(int index);
    
    private:
    std::vector<Action> printableKeys();
    std::vector<Action> functionalKeys();
    std::vector<Action> mouseButtons();
    std::vector<Action> gamepadButtons();
    
    std::vector<Action> actions;
    std::unordered_map<std::string, State> states;
    static std::vector<int> connectedJoysticks;
    
    static void joystick_callback(int jid, int event);
    void addConnectedJoysticks();

    template<typename... Args>
    bool checkState(const std::string &stateType, Args... args)
    {
      for(auto&& actionName : { args... })
      {
        if(this->states.find(actionName) == this->states.end()) continue;

        if(stateType == "held" && this->states[actionName].held) return true;
        if(stateType == "pressed" && this->states[actionName].pressed) return true;
        if(stateType == "released" && this->states[actionName].released) return true;
      }

      return false;
    }
  };

  inline Input input;
};