#pragma once

#include "../../ngynpch.hpp"

#include "window.hpp"

namespace ngyn
{
  class Engine
  {
    public:
    Engine();
    ~Engine();

    Window window;

    void run();
    void setup();
    void update();
    void render();

    virtual void onSetup() = 0;
    virtual void onUpdate() = 0;
    // Temporary maybe
    virtual void onRender() = 0;
  };
};