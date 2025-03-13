#include <ngyn/ngyn.hpp>

using namespace ngyn;

AnimatedSprite sprite;
AnimatedSprite sprite2;

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
    ngLogger.setFormat("$T");

    auto texture = ResourcesManager::addResource<Texture>("animated_ball", Texture{{
      .image = "data/textures/animated_ball.png",
      .filtering = Texture::Filtering::Nearest
    }});

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

    sprite = AnimatedSprite{{
      .frame = {
        .texture = texture,
        .index = 1,
        .size = glm::vec2(32.0f),
        .color = Color(255, 0, 0, 255)
      },
      .transform = {
        .size = glm::vec2(128.0f)
      },
      .renderer = quadRenderer,
      .camera = camera,
    }};

    sprite.setAnimation({
      .name = "animation_1",
      .duration = 1000.0f,
      .frames = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14},
      .repeat = true,
    });

    sprite.setAnimation({
      .name = "animation_2",
      .duration = 1000.0f,
      .frames = {15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28},
      .repeat = true,
    });

    sprite2 = sprite;

    sprite2.transform.setPosition(glm::vec2(200.0f));

    sprite.instantiate();
    sprite2.instantiate();

    quadRenderer.lock().get()->setup();
  };

  virtual void onUpdate()
  {
    float speed = 200.0f;

    if(ngInput.pressed("KEY_1"))
    {
      sprite.play("animation_1");
      sprite2.play("animation_2");
    }

    if(ngInput.pressed("KEY_2"))
    {
      sprite.play("animation_2");
      sprite2.play("animation_1");
    }

    if(ngInput.pressed("KEY_S"))
    {
      sprite.stop();
      sprite2.stop();
    }

    if(ngInput.pressed("KEY_P"))
    {
      sprite.toggle();
      sprite2.toggle();
    }

    sprite.update();
    sprite2.update();
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