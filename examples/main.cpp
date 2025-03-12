#include <ngyn/ngyn.hpp>

using namespace ngyn;

int main()
{
  logger.setFormat("$T");

  Window window({
    .resizable = true,
    .monitor = 1
  });

  Time time;
  Input input;

  auto texture = ResourcesManager::addResource<Texture>("animated_ball", Texture{{
    .image = "data/textures/animated_ball.png",
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

  AnimatedSprite sprite{{
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

  sprite.addAnimation({
    .name = "animation_1",
    .duration = 1000.0f,
    .frames = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14},
    .repeat = true,
  });

  sprite.addAnimation({
    .name = "animation_2",
    .duration = 1000.0f,
    .frames = {15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28},
    .repeat = true,
  });

  AnimatedSprite sprite2 = sprite;

  sprite2.transform.setPosition(glm::vec2(200.0f));

  sprite.instantiate();
  sprite2.instantiate();

  float speed = 200.0f;

  while(window.isOpen())
  {
    window.handleEvents();
    time.update();
    input.update(window.handle());

    if(input.pressed("KEY_1"))
    {
      sprite.play("animation_1");
      sprite2.play("animation_2");
    }

    if(input.pressed("KEY_2"))
    {
      sprite.play("animation_2");
      sprite2.play("animation_1");
    }

    if(input.pressed("KEY_S"))
    {
      sprite.stop();
      sprite2.stop();
    }

    if(input.pressed("KEY_P"))
    {
      sprite.toggle();
      sprite2.toggle();
    }

    sprite.update();
    sprite2.update();

    window.clear();

    quadRenderer.get()->render();

    window.swapBuffers();
  }
}