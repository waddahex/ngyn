#pragma once

#include "../../ngynpch.hpp"

#include "window.hpp"
#include "input.hpp"
#include "time.hpp"

namespace ngyn
{
  class Engine
  {
    struct CreateInfo
    {
      Window window;
    };

    public:
    Engine(CreateInfo createInfo);

    void run();
    void setup();
    void update();
    void render();

    virtual void onSetup() = 0;
    virtual void onUpdate() = 0;
    virtual void onRender() = 0;
    
    protected:
    Window window;
  };
};