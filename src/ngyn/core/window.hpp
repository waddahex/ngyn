#pragma once

#include <glm/glm.hpp>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <filesystem>

#include "../util/logger.hpp"
#include "../util/files.hpp"
#include <rapidjson/document.h>

namespace ngyn
{
  enum WindowMode
  {
    Fullscreen = 0,
    Windowed = 1,
    Borderless = 2
  };

  struct WindowCreateInfo
  {
    std::filesystem::path configPath;
    std::string title = "ngyn";
    glm::ivec2 dimensions = glm::ivec2(1280, 720);
    glm::ivec2 resolution = glm::ivec2(1280, 720);
    bool resizable = true;
    bool maximized = false;
    int monitor = 0;
    WindowMode mode = WindowMode::Windowed;
  };

  class Window
  {
    public:
    Window(WindowCreateInfo createInfo = WindowCreateInfo{});
    ~Window();

    GLFWwindow *handle;
    std::string title;
    glm::ivec2 dimensions;
    glm::ivec2 resolution;
    bool resizable;
    bool maximized;

    bool isOpen();
    void clear();
    void swapBuffers();
    void handleEvents();
    void setTitle(const std::string &newTitle);

    void destroy();

    static void glDebugOutput(GLenum src, GLenum type, GLuint id, GLenum severity, GLsizei len, const char *msg, const void *usrParam);

    private:
    WindowCreateInfo loadConfig(const std::filesystem::path &path);
  };
};