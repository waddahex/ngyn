#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <ngyn/ngyn.hpp>

using namespace ngyn;

TEST_CASE("Initialization")
{

  SUBCASE("Size should be equal the creation size")
  {
    glm::ivec2 creationSize = glm::ivec2(640, 360);
    Window window{{
      .size = creationSize
    }};

    glm::ivec2 windowSize;
    glfwGetWindowSize(window.handle(), &windowSize.x, &windowSize.y);

    CHECK(windowSize == creationSize);
  }

  SUBCASE("Should have the same width as the monitor")
  {
    Window window{{
      .maximized = true
    }};

    auto vidMode = glfwGetVideoMode(glfwGetPrimaryMonitor());

    glm::ivec2 windowSize;
    glfwGetWindowSize(window.handle(), &windowSize.x, &windowSize.y);

    CHECK(windowSize.x == vidMode->width);
  }

  SUBCASE("Should have the correct title")
  {
    Window window{{
      .title = "Test title"
    }};

    std::string windowTitle = glfwGetWindowTitle(window.handle());
    CHECK("Test title" == windowTitle);
  }

  SUBCASE("Should have the correct monitor")
  {
    glfwInit();

    int count;
    GLFWmonitor** monitors = glfwGetMonitors(&count);

    GLFWmonitor *monitor = count == 1 ?
      glfwGetPrimaryMonitor() :
      monitors[1];

    Window window{{
      .monitor = count == 1 ? 0 : 1 // use secondary monitor if there is one
    }};

    auto vidMode = glfwGetVideoMode(monitor);

    int xPos, yPos;
    glfwGetMonitorPos(monitor, &xPos, &yPos);

    // Get the top bar size
    int left, top , right, bottom;
    glfwGetWindowFrameSize(window.handle(), &left, &top, &right, &bottom);

    // Calculate position for the window to be centered on the monitor
    int xCenterPos = xPos + vidMode->width * 0.5f - window.size().x * 0.5f;
    int yCenterPos = yPos + vidMode->height * 0.5f - (window.size().y + top) * 0.5f;

    int windowX, windowY;
    glfwGetWindowPos(window.handle(), &windowX, &windowY);


    CHECK(glm::ivec2(xCenterPos, yCenterPos) == glm::ivec2(windowX, windowY));
  }


  SUBCASE("Should not be resizable")
  {
    Window window{{
      .resizable = false
    }};

    auto attribute = glfwGetWindowAttrib(window.handle(), GLFW_RESIZABLE);
    CHECK(attribute == GLFW_FALSE);
  }

  SUBCASE("Should use the config file")
  {
    std::string configJson = R"(
{
  "title": "Config window",
  "size": {
    "width": 640,
    "height": 360
  },
  "resolution": {
    "width": 1280,
    "height": 720
  },
  "monitor": 0,
  "resizable": false,
  "maximized": false
}
    )";

    files::write("windowConfig.json", configJson, {.recursive = true});

    Window window{{
      .configPath = "windowConfig.json",
      .title = "Test window",
      .size = glm::ivec2(1280, 720),
      .resizable = true
    }};

    auto attribute = glfwGetWindowAttrib(window.handle(), GLFW_RESIZABLE);
    CHECK(attribute == GLFW_FALSE);

    std::string windowTitle = glfwGetWindowTitle(window.handle());
    CHECK(windowTitle == "Config window");

    glm::ivec2 windowSize;
    glfwGetWindowSize(window.handle(), &windowSize.x, &windowSize.y);

    CHECK(windowSize == glm::ivec2(640, 360));
  }

  SUBCASE("Should use the provided info if config file doesn't exist")
  {
    Window window{{
      .configPath = "invalidWindowConfig.json",
      .title = "Test window",
      .size = glm::ivec2(800, 600),
      .resizable = false
    }};

    std::string windowTitle = glfwGetWindowTitle(window.handle());
    CHECK(windowTitle == "Test window");

    auto attribute = glfwGetWindowAttrib(window.handle(), GLFW_RESIZABLE);
    CHECK(attribute == GLFW_FALSE);

    glm::ivec2 windowSize;
    glfwGetWindowSize(window.handle(), &windowSize.x, &windowSize.y);
    CHECK(windowSize == glm::ivec2(800, 600));
  }
}

TEST_CASE("Window updates")
{
  SUBCASE("Title should be updated")
  {
    Window window{{}};

    std::string newTitle = "New title test";
    window.setTitle(newTitle);

    std::string windowTitle = glfwGetWindowTitle(window.handle());
    CHECK(newTitle == windowTitle);
  }

  SUBCASE("Color should be update")
  {
    Window window{{}};

    Color color;
    glGetFloatv(GL_COLOR_CLEAR_VALUE, &color[0]);

    window.setColor(Color(123));

    Color newColor;
    glGetFloatv(GL_COLOR_CLEAR_VALUE, &color[0]);

    CHECK(color != newColor);
  }
}