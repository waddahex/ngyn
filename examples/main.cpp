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

  auto texture = ResourcesManager::addResource<Texture>("arrows", Texture(TextureCreateInfo{
    .image = "data/textures/arrows.png",
    .filtering = TextureFiltering::Nearest
  }));

  std::shared_ptr<QuadRenderer> quadRenderer = std::make_shared<QuadRenderer>(QuadRenderer{});
  quadRenderer.get()->setup();

  std::shared_ptr<Camera> camera = std::make_shared<Camera>(Camera{{
    .position = glm::vec2(0.0f),
    .resolution = glm::vec2(1280.0f * 2, 720.0f * 2)
  }});

  int frameSize = 32;
  for(size_t x = 0; x < texture.get()->width / frameSize; x++)
  {
    for(size_t y = 0; y < texture.get()->height / frameSize; y++)
    {
      Sprite sprite{{
        .frame = {
          .texture = texture,
          .offset = glm::vec2(x * frameSize, y * frameSize),
          .size = glm::vec2(frameSize)
        },
        .transform = {
          .position = glm::vec2(x * 128.0f, y * 128.0f),
          .size = glm::vec2(128.0f)
        },
        .renderer = quadRenderer,
        .camera = camera
      }};
    
      sprite.instantiate();
    }
  }

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