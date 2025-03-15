#include <ngyn/ngyn.hpp>

using namespace ngyn;

Text text;

class Game : public Engine
{
  public:
  Game() : Engine({
    .window = Window{{
      .resizable = true,
      .monitor = 1
    }}
  }){};

  virtual void onSetup()
  {
    ngLogger.setFormat("HH:mm:ss");

    auto font = ResourcesManager::addResource<Font>("arial", Font{{
      .path = "data/fonts/arial.ttf",
      .name = "arial",
      .size = 24,
    }});

    auto camera = ResourcesManager::addResource<Camera>("main_camera", Camera{{
      .position = glm::vec2(0.0f),
      .resolution = window.resolution()
    }});

    auto quadRenderer = ResourcesManager::addResource<QuadRenderer>("quad_renderer", QuadRenderer{});

    text = Text{Text::CreateInfo{
      .font = font,
      .camera = camera,
      .renderer = quadRenderer,
      .position = glm::vec2(0.0f),
      .value = "Test value",
      .color = Color(255, 255, 0),
    }};

    text.instantiate();

    quadRenderer.lock().get()->setup();
  };

  virtual void onUpdate()
  {
    // if(ngTime.justUpdated())
    // {
    //   text.setValue(std::format("FPS: {} | MS: {} | DT: {}", ngTime.fps(), ngTime.ms(), ngTime.deltaTime()));
    //   LOGGER_DEBUG("FPS: {} | MS: {} | DT: {}", ngTime.fps(), ngTime.ms(), ngTime.deltaTime());
    // }

    if(ngInput.pressed("KEY_A"))
    {
      text.setValue("1234567890");
    }

    if(ngInput.pressed("KEY_S"))
    {
      text.setValue("12345");
    }

    if(ngInput.pressed("KEY_D"))
    {
      text.setValue("asdfqwkjehraskdjfh");
    }

    text.update();
  };

  virtual void onRender()
  {
    auto quadRenderer = ResourcesManager::getResource<QuadRenderer>("quad_renderer");
    quadRenderer.lock().get()->render();
  };
};

int main()
{
  Game game;
  game.run();
}