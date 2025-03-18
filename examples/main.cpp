#include <ngyn/ngyn.hpp>

using namespace ngyn;

Sprite s1;
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
    window.setIcon("data/icons/icon.png");

    ngLogger.setFormat("HH:mm:ss");

    auto font = ResourcesManager::addResource<Font>("arial", Font{{
      .path = "data/fonts/dogica.ttf",
      .name = "arial",
      .size = 16,
      .pixelated = true
    }});

    auto camera = ResourcesManager::addResource<Camera>("main_camera", Camera{{
      .position = glm::vec2(0.0f),
      .resolution = window.resolution()
    }});

    auto quadRenderer = ResourcesManager::addResource<QuadRenderer>("quad_renderer", QuadRenderer{});

    s1 = Sprite(Sprite::CreateInfo{
      .frame = {
        .color = Color(255, 0, 0)
      },
      .transform = {
        .position = glm::vec2(0.0f, 200.0f),
        .size = glm::vec2(100.0f)
      },
      .renderer = quadRenderer,
      .camera = camera,
    });

    // s1.instantiate();

    text = Text{Text::CreateInfo{
      .font = font,
      .camera = camera,
      .renderer = quadRenderer,
      .value = "Test value",
    }};

    text.instantiate();

    quadRenderer.lock().get()->setup();
  };

  virtual void onUpdate()
  {
    if(ngInput.pressed("KEY_A"))
    {
      text.setVisibility(Frame::Visibility::Hidden);
    }

    if(ngInput.pressed("KEY_S"))
    {
      text.setVisibility(Frame::Visibility::Visible);
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