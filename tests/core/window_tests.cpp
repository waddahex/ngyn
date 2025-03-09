#include <catch2/catch_test_macros.hpp>
#include <ngyn/ngyn.hpp>

using namespace ngyn;
using namespace std::chrono_literals;

TEST_CASE("Window initialization", "[window]")
{
  SECTION("Window should have the correct size")
  {
    glm::ivec2 createSize(640, 360);
    Window window(WindowCreateInfo{
      .dimensions = createSize,
      .mode = WindowMode::Windowed
    });

    glm::ivec2 windowSize;
    glfwGetWindowSize(window.handle, &windowSize.x, &windowSize.y);

    REQUIRE(windowSize == createSize);
    std::this_thread::sleep_for(std::chrono::duration<float, std::milli>(200));
  }

  SECTION("Window should have the same size as the monitor when on fullscreen")
  {
    glm::ivec2 createSize(640, 360);
    Window window(WindowCreateInfo{
      .dimensions = createSize,
      .monitor = 0,
      .mode = WindowMode::Fullscreen,
    });

    glm::ivec2 windowSize;
    glfwGetWindowSize(window.handle, &windowSize.x, &windowSize.y);

    auto monitor = glfwGetPrimaryMonitor();
    auto videoMode = glfwGetVideoMode(monitor);

    REQUIRE(windowSize == glm::ivec2(videoMode->width, videoMode->height));
    std::this_thread::sleep_for(std::chrono::duration<float, std::milli>(200));
  }

  SECTION("Window should have the same size as the monitor when on borderless")
  {
    glm::ivec2 createSize(640, 360);
    Window window(WindowCreateInfo{
      .dimensions = createSize,
      .monitor = 0,
      .mode = WindowMode::Borderless,
    });

    glm::ivec2 windowSize;
    glfwGetWindowSize(window.handle, &windowSize.x, &windowSize.y);

    auto monitor = glfwGetPrimaryMonitor();
    auto videoMode = glfwGetVideoMode(monitor);

    REQUIRE(windowSize == glm::ivec2(videoMode->width, videoMode->height));
    std::this_thread::sleep_for(std::chrono::duration<float, std::milli>(200));
  }

  SECTION("Window should be maximized")
  {
    glm::ivec2 createSize(640, 360);
    Window window(WindowCreateInfo{
      .maximized = true,
      .mode = WindowMode::Windowed,
    });

    auto attribute = glfwGetWindowAttrib(window.handle, GLFW_MAXIMIZED);
    REQUIRE(attribute == GLFW_TRUE);
    std::this_thread::sleep_for(std::chrono::duration<float, std::milli>(200));
  }

  SECTION("Window should have the correct title")
  {
    std::string createTitle = "Test window";
    Window window(WindowCreateInfo{
      .title = createTitle
    });

    std::string windowTitle = glfwGetWindowTitle(window.handle);
    REQUIRE(createTitle == windowTitle);
    std::this_thread::sleep_for(std::chrono::duration<float, std::milli>(200));
  }

  SECTION("Window should have the correct monitor")
  {
    glfwInit();

    int count;
    GLFWmonitor** monitors = glfwGetMonitors(&count);

    GLFWmonitor *monitor = count == 1 ?
      glfwGetPrimaryMonitor() :
      monitors[1];

    Window window(WindowCreateInfo{
      .monitor = count == 1 ? 0 : 1, // use secondary monitor if there is one
      .mode = WindowMode::Fullscreen
    });

    auto windowMonitor = glfwGetWindowMonitor(window.handle);
    REQUIRE(windowMonitor == monitor);
    std::this_thread::sleep_for(std::chrono::duration<float, std::milli>(200));
  }

  SECTION("Window should not be resizable")
  {
    Window window(WindowCreateInfo{
      .resizable = false
    });

    auto attribute = glfwGetWindowAttrib(window.handle, GLFW_RESIZABLE);
    REQUIRE(attribute == GLFW_FALSE);
    std::this_thread::sleep_for(std::chrono::duration<float, std::milli>(200));
  }

  SECTION("Window should use the config file")
  {
    std::string configJson = R"(
{
  "title": "Config window",
  "dimensions": {
    "width": 640,
    "height": 360
  },
  "resolution": {
    "width": 1280,
    "height": 720
  },
  "monitor": 0,
  "mode": 1,
  "resizable": false,
  "maximized": false
}
    )";

    files::write("windowConfig.json", configJson, {.recursive = true});

    Window window(WindowCreateInfo{
      .configPath = "windowConfig.json",
      .title = "Test window",
      .dimensions = glm::ivec2(1280, 720),
      .resizable = true
    });

    auto attribute = glfwGetWindowAttrib(window.handle, GLFW_RESIZABLE);
    REQUIRE(attribute == GLFW_FALSE);

    std::string windowTitle = glfwGetWindowTitle(window.handle);
    REQUIRE(windowTitle == "Config window");

    glm::ivec2 windowSize;
    glfwGetWindowSize(window.handle, &windowSize.x, &windowSize.y);
    REQUIRE(windowSize == glm::ivec2(640, 360));
    std::this_thread::sleep_for(std::chrono::duration<float, std::milli>(200));
  }

  SECTION("Window should use the provided info if config file doesn't exist")
  {
    Window window(WindowCreateInfo{
      .configPath = "invalidWindowConfig.json",
      .title = "Test window",
      .dimensions = glm::ivec2(800, 600),
      .resizable = false
    });

    std::string windowTitle = glfwGetWindowTitle(window.handle);
    REQUIRE(windowTitle == "Test window");

    auto attribute = glfwGetWindowAttrib(window.handle, GLFW_RESIZABLE);
    REQUIRE(attribute == GLFW_FALSE);

    glm::ivec2 windowSize;
    glfwGetWindowSize(window.handle, &windowSize.x, &windowSize.y);
    REQUIRE(windowSize == glm::ivec2(800, 600));
    std::this_thread::sleep_for(std::chrono::duration<float, std::milli>(200));
  }

  SECTION("Window should use primary monitor if monitor used doesnt' exist")
  {
    Window window(WindowCreateInfo{
      .monitor = 99,
      .mode = WindowMode::Fullscreen
    });

    auto monitor = glfwGetWindowMonitor(window.handle);

    REQUIRE(monitor == glfwGetPrimaryMonitor());
    std::this_thread::sleep_for(std::chrono::duration<float, std::milli>(200));
  }
}


TEST_CASE("Window updates", "[window]")
{
  SECTION("Window title should be updated")
  {
    Window window(WindowCreateInfo{});

    std::string newTitle = "New title test";
    window.setTitle(newTitle);

    std::string windowTitle = glfwGetWindowTitle(window.handle);
    REQUIRE(newTitle == windowTitle);
    std::this_thread::sleep_for(std::chrono::duration<float, std::milli>(200));
  }

  SECTION("Window color should be update")
  {
    Window window(WindowCreateInfo{});

    Color color;
    glGetFloatv(GL_COLOR_CLEAR_VALUE, &color[0]);

    window.setColor(Color(123));

    Color newColor;
    glGetFloatv(GL_COLOR_CLEAR_VALUE, &color[0]);

    REQUIRE(color != newColor);
  }
}