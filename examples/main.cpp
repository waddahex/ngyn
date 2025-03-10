#include <ngyn/ngyn.hpp>

using namespace ngyn;

int main()
{
  logger.setFormat("$T");

  Window window({
    .monitor = 1
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

  std::shared_ptr<QuadRenderer> quadRenderer = std::make_shared<QuadRenderer>(QuadRenderer{});
  quadRenderer.get()->setup();

  std::shared_ptr<Camera> camera = std::make_shared<Camera>(Camera{{
    .position = glm::vec2(0.0f),
    .resolution = glm::vec2(window.resolution.x, window.resolution.y)
  }});

  Text text{{
    .font = font,
    .camera = camera,
    .renderer = quadRenderer,
    .position = glm::vec2(0.0f),
    .rotation = 0.0f,
    .value = "Hello world",
  }};

  text.instantiate();

  float speed = 200.0f;
  while(window.isOpen())
  {
    window.handleEvents();
    time.update();
    input.update(window);

    window.clear();

    quadRenderer.get()->render();

    window.swapBuffers();
  }
}