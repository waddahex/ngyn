#include "window.hpp"

using namespace ngyn;

Window::Window(WindowCreateInfo createInfo)
{
  if(!createInfo.configPath.empty())
  {
    WindowCreateInfo loadedConfig = this->loadConfig(createInfo.configPath);
    if(!loadedConfig.configPath.empty())
    {
      createInfo = loadedConfig;
    }
  }

  this->dimensions = createInfo.dimensions;
  this->resolution = createInfo.resolution;
  this->resizable = createInfo.resizable;
  this->maximized = createInfo.maximized;
  this->title = createInfo.title;

  ASSERT(glfwInit(), "Failed to initialized GLFW");

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  
  #ifndef NDEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
  #endif

  if(!this->resizable)
  {
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  }

  int count;
  GLFWmonitor** monitors = glfwGetMonitors(&count);

  GLFWmonitor *monitor = createInfo.monitor > count -1 ?
    glfwGetPrimaryMonitor() :
    monitors[createInfo.monitor];

  const GLFWvidmode *videoMode = glfwGetVideoMode(monitor);

  this->handle = glfwCreateWindow(
    createInfo.mode == WindowMode::Fullscreen ? videoMode->width : this->dimensions.x,
    createInfo.mode == WindowMode::Fullscreen ? videoMode->height : this->dimensions.y,
    createInfo.title.c_str(),
    createInfo.mode == WindowMode::Fullscreen ? monitor : nullptr,
    nullptr
  );

  // Center window on the selected monitor
  if(createInfo.mode == WindowMode::Windowed)
  {
    int xPos, yPos;
    glfwGetMonitorPos(monitor, &xPos, &yPos);

    int topbarSize = 30;

    int xCenterPos = xPos + videoMode->width * 0.5f - this->dimensions.x * 0.5f;
    int yCenterPos = yPos + videoMode->height * 0.5f - (this->dimensions.y + topbarSize) * 0.5f;

    glfwSetWindowPos(this->handle, xCenterPos, yCenterPos);
  }

  if(createInfo.mode == WindowMode::Borderless)
  {
    glfwSetWindowMonitor(
      this->handle,
      monitor,
      0,
      0,
      videoMode->width,
      videoMode->height,
      videoMode->refreshRate
    );
  }

  if(this->maximized)
  {
    glfwMaximizeWindow(this->handle);
  }

  glfwMakeContextCurrent(this->handle);
  ASSERT(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress), "Failed to initialize OpenGL");

  int flags;
  glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
  if(flags & GL_CONTEXT_FLAG_DEBUG_BIT)
  {
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(this->glDebugOutput, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
  }

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

Window::~Window()
{
  this->destroy();
}

bool Window::isOpen()
{
  return !glfwWindowShouldClose(this->handle);
}

void Window::clear()
{
  glClear(GL_COLOR_BUFFER_BIT);
}

void Window::swapBuffers()
{
  glfwSwapBuffers(this->handle);
}

void Window::handleEvents()
{
  glfwPollEvents();
}

void Window::setTitle(const std::string &newTitle)
{
  glfwSetWindowTitle(this->handle, newTitle.c_str());
}

void Window::destroy()
{
  glfwDestroyWindow(this->handle);
  glfwTerminate();
}

WindowCreateInfo Window::loadConfig(const std::filesystem::path &path)
{
  WindowCreateInfo createInfo;

  std::string data = files::read(path);

  if(data.empty()) return createInfo;

  createInfo.configPath = path.string();

  rapidjson::Document json;
  json.Parse(data.c_str());

  if(json.HasMember("title") && json["title"].IsString())
  {
    createInfo.title = json["title"].GetString();
  }

  if(json.HasMember("dimensions") && json["dimensions"].IsObject())
  {
    auto dimensions = json["dimensions"].GetObject();

    if(
      dimensions.HasMember("width") && dimensions["width"].IsInt() &&
      dimensions.HasMember("height") && dimensions["height"].IsInt()
    )
    {
      createInfo.dimensions = glm::ivec2(
        dimensions["width"].GetInt(),
        dimensions["height"].GetInt()
      );
    }
  }

  if(json.HasMember("resolution") && json["resolution"].IsObject())
  {
    auto resolution = json["resolution"].GetObject();

    if(
      resolution.HasMember("width") && resolution["width"].IsInt() &&
      resolution.HasMember("height") && resolution["height"].IsInt()
    )
    {
      createInfo.resolution = glm::ivec2(
        resolution["width"].GetInt(),
        resolution["height"].GetInt()
      );
    }
  }

  if(json.HasMember("resizable") && json["resizable"].IsBool())
  {
    createInfo.resizable = json["resizable"].GetBool();
  }

  if(json.HasMember("maximized") && json["maximized"].IsBool())
  {
    createInfo.maximized = json["maximized"].GetBool();
  }

  if(json.HasMember("monitor") && json["monitor"].IsInt())
  {
    createInfo.monitor = json["monitor"].GetInt();
  }

  if(json.HasMember("mode") && json["mode"].IsInt())
  {
    createInfo.mode = static_cast<WindowMode>(json["mode"].GetInt());
  }

  return createInfo;
}

void Window::glDebugOutput(GLenum src, GLenum type, GLuint id, GLenum severity, GLsizei len, const char *msg, const void *usrParam)
{
  // ignore non-significant error/warning codes
  if(id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

  std::ostringstream sstream;

  sstream << "\nDebug message (" << id << "): " << msg << "\n";

  switch (src)
  {
    case GL_DEBUG_SOURCE_API:             sstream << "Source: API\n"; break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   sstream << "Source: Window System\n"; break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER: sstream << "Source: Shader Compiler\n"; break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:     sstream << "Source: Third Party\n"; break;
    case GL_DEBUG_SOURCE_APPLICATION:     sstream << "Source: Application\n"; break;
    case GL_DEBUG_SOURCE_OTHER:           sstream << "Source: Other\n"; break;
  }

  switch (type)
  {
    case GL_DEBUG_TYPE_ERROR:               sstream << "Type: Error\n"; break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: sstream << "Type: Deprecated Behaviour\n"; break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  sstream << "Type: Undefined Behaviour\n"; break; 
    case GL_DEBUG_TYPE_PORTABILITY:         sstream << "Type: Portability\n"; break;
    case GL_DEBUG_TYPE_PERFORMANCE:         sstream << "Type: Performance\n"; break;
    case GL_DEBUG_TYPE_MARKER:              sstream << "Type: Marker\n"; break;
    case GL_DEBUG_TYPE_PUSH_GROUP:          sstream << "Type: Push Group\n"; break;
    case GL_DEBUG_TYPE_POP_GROUP:           sstream << "Type: Pop Group\n"; break;
    case GL_DEBUG_TYPE_OTHER:               sstream << "Type: Other\n"; break;
  }
  
  switch (severity)
  {
    case GL_DEBUG_SEVERITY_HIGH:         sstream << "Severity: high\n"; break;
    case GL_DEBUG_SEVERITY_MEDIUM:       sstream << "Severity: medium\n"; break;
    case GL_DEBUG_SEVERITY_LOW:          sstream << "Severity: low\n"; break;
    case GL_DEBUG_SEVERITY_NOTIFICATION: sstream << "Severity: notification\n"; break;
  }

  LOGGER_ERROR(sstream.str());
}