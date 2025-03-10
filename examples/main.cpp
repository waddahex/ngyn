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

  std::shared_ptr<QuadRenderer> quadRenderer = std::make_shared<QuadRenderer>(QuadRenderer{});
  quadRenderer.get()->setup();

  std::shared_ptr<Camera> camera = std::make_shared<Camera>(Camera{{
    .position = glm::vec2(0.0f),
    .resolution = glm::vec2(window.resolution.x, window.resolution.y)
  }});

  Sprite sprite{{
    .frame = {
      .texture = texture,
      .offset = glm::vec2(32.0f),
      .size = glm::vec2(32.0f),
    },
    .transform = {
      .position = glm::vec2(0.0f),
      .size = glm::vec2(128.0f)
    },
    .renderer = quadRenderer,
    .camera = camera
  }};

  sprite.instantiate();

  float speed = 200.0f;
  while(window.isOpen())
  {
    window.handleEvents();
    time.update();
    input.update(window);

    glm::vec2 velocity(0.0f);

    if(input.held("KEY_A", "KEY_LEFT"))
    {
      velocity.x -= speed * time.deltaTime;
    }

    if(input.held("KEY_D", "KEY_RIGHT"))
    {
      velocity.x += speed * time.deltaTime;
    }

    sprite.moveBy(velocity);

    sprite.update();

    window.clear();

    quadRenderer.get()->render();

    window.swapBuffers();
  }
}