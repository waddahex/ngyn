#include "engine.hpp"

ddengine::Engine::Engine()
{
  this->window = Window(WindowCreateInfo{
    .name = "Window from engine",
    .dimensions = glm::ivec2(640, 360),
  });
}

ddengine::Engine::~Engine()
{
  this->window.destroy();
}

void ddengine::Engine::run()
{
  this->setup();

  while(window.isOpen())
  {
    this->update();

    window.clear();
    this->render();
    window.swapBuffers();
  }
}

void ddengine::Engine::setup()
{
  this->onSetup();
}

void ddengine::Engine::update()
{
  window.handleEvents();

  this->onUpdate();
}

void ddengine::Engine::render()
{
  this->onRender();
}