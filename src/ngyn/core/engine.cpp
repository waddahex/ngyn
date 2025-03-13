#include "engine.hpp"

using namespace ngyn;

Engine::Engine(CreateInfo createInfo) :
  window(createInfo.window)
{
}

void Engine::run()
{
  setup();

  while(window.isOpen())
  {
    update();
    render();
  }

  window.destroy();
}

void Engine::setup()
{
  window.open();
  window.loadGL();

  onSetup();
}

void Engine::update()
{
  window.handleEvents();
  ngTime.update();
  ngInput.update(window.handle());

  onUpdate();
}

void Engine::render()
{
  window.clear();
  onRender();
  window.swapBuffers();
}