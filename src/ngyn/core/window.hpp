#pragma once

#include "../../ngynpch.hpp"

#include "../util/logger.hpp"
#include "../util/files.hpp"
#include "../util/color.hpp"
#include "../components/transform.hpp"
#include "../components/camera.hpp"
#include "mouse.hpp"

namespace ngyn
{
  class Window
  {
    public:
    struct CreateInfo
    {
      std::filesystem::path configPath;
      std::string title = "ngyn";
      glm::ivec2 size = glm::ivec2(1280, 720);
      glm::ivec2 resolution = glm::ivec2(1280, 720);
      bool resizable = true;
      bool maximized = false;
      int monitor = 0;
    };

    public:
    Window(CreateInfo createInfo = CreateInfo{});

    GLFWwindow *handle();
    const glm::ivec2 &size();
    const glm::ivec2 &resolution();
    std::shared_ptr<Camera> camera();
    std::shared_ptr<Transform> transform();

    void open();
    void close();
    void loadGL();
    bool isOpen();
    void clear();
    void swapBuffers();
    void handleEvents();
    void setSize(const glm::ivec2 &size);
    void setTitle(const std::string &newTitle);
    void setColor(const Color &color);

    void destroy();

    static void framebufferSizeCallback(GLFWwindow *handle, int width, int height);
    static void cursorPosCallback(GLFWwindow *handle, double xPos, double yPos);
    static void glDebugOutput(GLenum src, GLenum type, GLuint id, GLenum severity, GLsizei len, const char *msg, const void *usrParam);

    private:
    GLFWwindow *_handle;
    std::string _title;
    glm::ivec2 _size;
    glm::ivec2 _resolution;
    bool _resizable;
    bool _maximized;
    int _monitor;
    std::shared_ptr<Camera> _camera;
    std::shared_ptr<Transform> _transform;

    bool _glLoaded;

    void loadConfig(const std::filesystem::path &path);
    static glm::ivec4 getViewportDimension(const glm::ivec2 &size, const glm::ivec2 &resolution);
  };
};