#include <ngyn/ngyn.hpp>

using namespace ngyn;

int main()
{
  logger.setFormat("$T");

  Window window({
    .resolution = glm::ivec2(3840, 2160),
    .resizable = true,
    .monitor = 1,
    .mode = Window::Mode::Windowed,
    .aspectRatio = glm::ivec2(0, 0)
  });

  Time time;
  Input input;

  auto texture = ResourcesManager::addResource<Texture>("arrows", Texture{{
    .image = "data/textures/arrows.png",
    .filtering = Texture::Filtering::Nearest
  }});

  auto font = ResourcesManager::addResource<Font>("arial", Font{{
    .path = "data/fonts/arial.ttf",
    .name = "arial",
    .size = 24,
  }});

  auto camera = std::make_shared<Camera>(Camera{{
    .position = glm::vec2(0.0f),
    .resolution = window.resolution()
  }});

  std::shared_ptr<QuadRenderer> quadRenderer = std::make_shared<QuadRenderer>(QuadRenderer{});
  quadRenderer.get()->setup();

  Sprite sprite{{
    .frame = {
      .color = Color(255, 0, 0, 255)
    },
    .transform = {
      .size = glm::vec2(128.0f)
    },
    .renderer = quadRenderer,
    .camera = camera,
  }};

  Sprite sprite2{{
    .frame = {
      .color = Color(255, 255, 0, 255)
    },
    .transform = {
      .position = glm::vec2(
        window.resolution().x - 128,
        window.resolution().y - 128
      ),
      .size = glm::vec2(128.0f)
    },
    .renderer = quadRenderer,
    .camera = camera,
  }};

  sprite.instantiate();
  sprite2.instantiate();

  // Text text{{
  //   .font = font,
  //   .camera = window.camera(),
  //   .renderer = quadRenderer,
  //   .position = glm::vec2(0.0f),
  //   .rotation = 0.0f,
  //   .value = "Hello world",
  // }};

  // text.instantiate();

  float speed = 200.0f;
  while(window.isOpen())
  {
    window.handleEvents();
    time.update();
    input.update(window.handle());

    window.clear();

    quadRenderer.get()->render();

    window.swapBuffers();
  }
}