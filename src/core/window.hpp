#pragma once

#include <glm/glm.hpp>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "../util/logger.hpp"

namespace ddengine
{
  struct WindowCreateInfo
  {
    glm::ivec2 dimensions = glm::ivec2(1280, 720);
    glm::ivec2 resolution = glm::ivec2(1280, 720);
    const char* name = "DD Engine";
    bool resizable = true;
    bool maximized = false;
  };

  class Window
  {
    public:
    Window(void) = default;
    Window(WindowCreateInfo createInfo);

    GLFWwindow *handle;
    glm::ivec2 dimensions;
    glm::ivec2 resolution;
    bool resizable;
    bool maximized;

    bool isOpen();
    void clear();
    void swapBuffers();
    void handleEvents();

    void destroy();
  };
};