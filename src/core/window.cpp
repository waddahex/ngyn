#include "window.hpp"

ddengine::Window::Window(WindowCreateInfo createInfo)
{
  this->dimensions = createInfo.dimensions;
  this->resolution = createInfo.resolution;
  this->resizable = createInfo.resizable;
  this->maximized = createInfo.maximized;

  glfwInit();

  logger.log("GLFW Initialized");

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  if(!this->resizable)
  {
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    logger.log("Window resizable: False");
  }

  if(this->maximized)
  {
    glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
    logger.log("Window maximized: True");
  }

  this->handle = glfwCreateWindow(
    this->dimensions.x,
    this->dimensions.y,
    createInfo.name,
    nullptr,
    nullptr
  );

  logger.log("Window created");

  glfwMakeContextCurrent(this->handle);
  gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

  logger.log("OpenGL Loaded");
}

bool ddengine::Window::isOpen()
{
  return !glfwWindowShouldClose(this->handle);
}

void ddengine::Window::clear()
{
  glClear(GL_COLOR_BUFFER_BIT);
}

void ddengine::Window::swapBuffers()
{
  glfwSwapBuffers(this->handle);
}

void ddengine::Window::handleEvents()
{
  glfwPollEvents();
}

void ddengine::Window::destroy()
{
  glfwDestroyWindow(this->handle);
  glfwTerminate();

  logger.log("Window destroyed");
}
