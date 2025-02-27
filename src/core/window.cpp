#include "window.hpp"

ngyn::Window::Window(WindowCreateInfo createInfo)
{
  this->dimensions = createInfo.dimensions;
  this->resolution = createInfo.resolution;
  this->resizable = createInfo.resizable;
  this->maximized = createInfo.maximized;

  glfwInit();

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  if(!this->resizable)
  {
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  }

  if(this->maximized)
  {
    glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
  }

  this->handle = glfwCreateWindow(
    this->dimensions.x,
    this->dimensions.y,
    createInfo.name,
    nullptr,
    nullptr
  );

  glfwMakeContextCurrent(this->handle);
  gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
}

bool ngyn::Window::isOpen()
{
  return !glfwWindowShouldClose(this->handle);
}

void ngyn::Window::clear()
{
  glClear(GL_COLOR_BUFFER_BIT);
}

void ngyn::Window::swapBuffers()
{
  glfwSwapBuffers(this->handle);
}

void ngyn::Window::handleEvents()
{
  glfwPollEvents();
}

void ngyn::Window::destroy()
{
  glfwDestroyWindow(this->handle);
  glfwTerminate();
}
