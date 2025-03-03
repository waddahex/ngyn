#include "engine.hpp"

ngyn::Engine::Engine()
{
  this->window = Window(WindowCreateInfo{
    .title = "Window from engine",
    .dimensions = glm::ivec2(640, 360),
  });
}

ngyn::Engine::~Engine()
{
  this->window.destroy();
}

void ngyn::Engine::run()
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

void ngyn::Engine::setup()
{
  this->onSetup();
}

void ngyn::Engine::update()
{
  window.handleEvents();

  this->onUpdate();
}

void ngyn::Engine::render()
{
  this->onRender();
}