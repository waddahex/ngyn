#pragma once

#include "../../ngynpch.hpp"

#include "../util/logger.hpp"
#include "../util/files.hpp"
#include "../util/color.hpp"
#include "../components/transform.hpp"
#include "../components/camera.hpp"

namespace ngyn
{
  class Window
  {
    public:
    enum Mode
    {
      Fullscreen = 0,
      Windowed = 1,
      Borderless = 2
    };
  
    struct CreateInfo
    {
      std::filesystem::path configPath;
      std::string title = "ngyn";
      glm::ivec2 size = glm::ivec2(1280, 720);
      glm::ivec2 resolution = glm::ivec2(1280, 720);
      bool resizable = true;
      bool maximized = false;
      int monitor = 0;
      Mode mode = Mode::Windowed;
      glm::ivec2 aspectRatio = glm::ivec2(16, 9);
    };

    public:
    Window(CreateInfo createInfo = CreateInfo{});
    ~Window();

    GLFWwindow *handle();
    const std::string &title();
    const glm::ivec2 &size();
    const glm::ivec2 &resolution();
    const bool &resizable();
    const bool &maximized();
    const int &monitor();
    const Mode &mode();
    std::shared_ptr<Camera> camera();
    std::shared_ptr<Transform> transform();

    bool isOpen();
    void clear();
    void swapBuffers();
    void handleEvents();
    void setSize(const glm::ivec2 &size);
    void setTitle(const std::string &newTitle);
    void setColor(const Color &color);
    void setAspectRatio(const glm::ivec2 &aspectRatio);
    void setMode(const Mode &mode);

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
    Mode _mode;
    std::shared_ptr<Camera> _camera;
    std::shared_ptr<Camera> _viewportCamera;
    std::shared_ptr<Transform> _transform;
    std::shared_ptr<Transform> _viewportTransform;

    CreateInfo loadConfig(const std::filesystem::path &path);
  };
};