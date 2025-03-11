#include "window.hpp"

using namespace ngyn;

Window::Window(CreateInfo createInfo) :
  _handle(nullptr),
  _title(createInfo.title),
  _size(createInfo.size),
  _resolution(createInfo.resolution),
  _resizable(createInfo.resizable),
  _maximized(createInfo.maximized),
  _monitor(createInfo.monitor),
  _mode(createInfo.mode)
{
  if(!createInfo.configPath.empty())
  {
    CreateInfo loadedConfig = this->loadConfig(createInfo.configPath);

    if(!loadedConfig.configPath.empty())
    {
      createInfo = loadedConfig;
    }
  }

  _camera = std::make_shared<Camera>(Camera{{
    .position = glm::vec2(0.0f),
    .resolution = _size
  }});

  _viewportCamera = std::make_shared<Camera>(Camera{{
    .position = glm::vec2(0.0f),
    .resolution = _resolution
  }});

  _transform = std::make_shared<Transform>(Transform{{
    .size = _size
  }});

  _viewportTransform = std::make_shared<Transform>(Transform{{
    .size = _resolution
  }});

  ASSERT(glfwInit(), "Failed to initialized GLFW");

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  
  #ifndef NDEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
  #endif

  if(!_resizable)
  {
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  }

  int count;
  GLFWmonitor** monitors = glfwGetMonitors(&count);

  GLFWmonitor *monitor = createInfo.monitor > count -1 ?
    glfwGetPrimaryMonitor() :
    monitors[createInfo.monitor];

  const GLFWvidmode *videoMode = glfwGetVideoMode(monitor);

  _handle = glfwCreateWindow(
    _size.x,
    _size.y,
    createInfo.title.c_str(),
    createInfo.mode == Mode::Fullscreen ? monitor : nullptr,
    nullptr
  );

  setMode(_mode);

  if(_maximized)
  {
    glfwMaximizeWindow(_handle);
  }

  setAspectRatio(createInfo.aspectRatio);

  glfwMakeContextCurrent(_handle);
  ASSERT(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress), "Failed to initialize OpenGL");

  int flags;
  glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
  if(flags & GL_CONTEXT_FLAG_DEBUG_BIT)
  {
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(glDebugOutput, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
  }

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glfwSetWindowUserPointer(_handle, this);
  glfwSetFramebufferSizeCallback(_handle, framebufferSizeCallback);
  glfwSetCursorPosCallback(_handle, cursorPosCallback);
}

Window::~Window()
{
  destroy();
}

GLFWwindow *Window::handle()
{
  return _handle;
}

const std::string &Window::title()
{
  return _title;
}

const glm::ivec2 &Window::size()
{
  return _size;
}

const glm::ivec2 &Window::resolution()
{
  return _resolution;
}

const bool &Window::resizable()
{
  return _resizable;
}

const bool &Window::maximized()
{
  return _maximized;
}

const int &Window::monitor()
{
  return _monitor;
}

const Window::Mode &Window::mode()
{
  return _mode;
}

std::shared_ptr<Camera> Window::camera()
{
  return _camera;
}

std::shared_ptr<Transform> Window::transform()
{
  return _transform;
}

bool Window::isOpen()
{
  return !glfwWindowShouldClose(_handle);
}

void Window::clear()
{
  glClear(GL_COLOR_BUFFER_BIT);
}

void Window::swapBuffers()
{
  glfwSwapBuffers(_handle);
}

void Window::handleEvents()
{
  glfwPollEvents();
}

void Window::setSize(const glm::ivec2 &size)
{
  _size = size;
}

void Window::setTitle(const std::string &newTitle)
{
  glfwSetWindowTitle(_handle, newTitle.c_str());
}

void Window::setColor(const Color &color)
{
  glClearColor(color.r, color.g, color.b, color.a);
}

void ngyn::Window::setAspectRatio(const glm::ivec2 & aspectRatio)
{
  glfwSetWindowAspectRatio(
    _handle,
    aspectRatio.x == 0 ? GLFW_DONT_CARE : aspectRatio.x,
    aspectRatio.y == 0 ? GLFW_DONT_CARE : aspectRatio.y
  );
}

void ngyn::Window::setMode(const Mode &mode)
{
  int count;
  GLFWmonitor** monitors = glfwGetMonitors(&count);

  GLFWmonitor *monitor = _monitor > count -1 ?
    glfwGetPrimaryMonitor() :
    monitors[_monitor];

  const GLFWvidmode *videoMode = glfwGetVideoMode(monitor);

  // Uses the selected resolution
  if(_mode == Mode::Fullscreen)
  {
    glfwSetWindowMonitor(
      _handle,
      monitor,
      0,
      0,
      _resolution.x,
      _resolution.y,
      videoMode->refreshRate
    );

    glfwSetInputMode(_handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  }

  if(mode == Mode::Windowed)
  {
    int xPos, yPos;
    glfwGetMonitorPos(monitor, &xPos, &yPos);

    int left, top , right, bottom;
    glfwGetWindowFrameSize(_handle, &left, &top, &right, &bottom);

    int xCenterPos = xPos + videoMode->width * 0.5f - _size.x * 0.5f;
    int yCenterPos = yPos + videoMode->height * 0.5f - (_size.y + top) * 0.5f;

    glfwSetWindowPos(_handle, xCenterPos, yCenterPos);
  }

  // uses monitor resolution (windowed fullscreen)
  if(_mode == Mode::Borderless)
  {
    glfwSetWindowMonitor(
      _handle,
      monitor,
      0,
      0,
      videoMode->width,
      videoMode->height,
      videoMode->refreshRate
    );
  }
}


void Window::destroy()
{
  glfwDestroyWindow(_handle);
  glfwTerminate();
}

void Window::framebufferSizeCallback(GLFWwindow *handle, int width, int height)
{
  Window *window = static_cast<Window*>(glfwGetWindowUserPointer(handle));

  ASSERT(window, "GLFW window user pointer (Window) not set");

  window->setSize(glm::ivec2(width, height));

  auto size = window->size();

  auto camera = window->camera().get();
  auto transform = window->transform().get();

  camera->setResolution(size);
  transform->setSize(size);

  auto resolution = window->resolution();

  glm::vec2 scale = glm::vec2(size) / glm::vec2(resolution);

  float resolutionAspect = static_cast<float>(resolution.x) / resolution.y;
  float sizeAspect = static_cast<float>(size.x) / size.y;

  glm::ivec2 viewportSize;

  if(sizeAspect > resolutionAspect)
  {
    viewportSize.y = size.y;
    viewportSize.x = static_cast<int>(size.y * resolutionAspect);
  }
  else
  {
    viewportSize.x = size.x;
    viewportSize.y = static_cast<int>(size.x / resolutionAspect);
  }

  glm::ivec2 viewportPosition;
  viewportPosition.x = (size.x - viewportSize.x) / 2;
  viewportPosition.y = (size.y - viewportSize.y) / 2;

  glViewport(
    viewportPosition.x,
    viewportPosition.y,
    viewportSize.x,
    viewportSize.y
  );
}

void ngyn::Window::cursorPosCallback(GLFWwindow *handle, double xPos, double yPos)
{
  Window *window = static_cast<Window*>(glfwGetWindowUserPointer(handle));

  ASSERT(window, "GLFW window user pointer (Window) not set");

  double finalX = xPos;
  double finalY = yPos;

  if(xPos > window->size().x) finalX = window->size().x;
  if(yPos > window->size().y) finalY = window->size().y;

  LOGGER_DEBUG("({}, {}) - {}", xPos, yPos, window->size());
}

Window::CreateInfo Window::loadConfig(const std::filesystem::path &path)
{
  CreateInfo createInfo;

  std::string data = files::read(path);

  if(data.empty()) return createInfo;

  createInfo.configPath = path.string();

  rapidjson::Document json;
  json.Parse(data.c_str());

  if(json.HasMember("title") && json["title"].IsString())
  {
    createInfo.title = json["title"].GetString();
  }

  if(json.HasMember("size") && json["size"].IsObject())
  {
    auto size = json["size"].GetObject();

    if(
      size.HasMember("width") && size["width"].IsInt() &&
      size.HasMember("height") && size["height"].IsInt()
    )
    {
      createInfo.size = glm::ivec2(
        size["width"].GetInt(),
        size["height"].GetInt()
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
    createInfo.mode = static_cast<Mode>(json["mode"].GetInt());
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