#define DOCTEST_CONFIG_IMPLEMENT
#include <doctest/doctest.h>

#include <ngyn/ngyn.hpp>

using namespace ngyn;

std::string dataPath = "data";

int main(int argc, char **argv)
{
  Window window{{}};
  window.open();
  window.loadGL();

  if(argc > 1)
  {
    dataPath = (std::filesystem::path(argv[1]) / "data").string();
  }

  doctest::Context context;

  context.applyCommandLine(argc, argv);

  int res = context.run();

  if(context.shouldExit()) return res;

  return res;
}

TEST_CASE("Basic tests")
{
  SUBCASE("Should set UUID on instantiate")
  {
    std::shared_ptr<QuadRenderer> quadRenderer = std::make_shared<QuadRenderer>(QuadRenderer{});
    std::shared_ptr<Camera> camera = std::make_shared<Camera>(Camera());
    std::shared_ptr<Texture> texture = std::make_shared<Texture>(Texture{{
      .image = dataPath + "/textures/animated_ball.png",
      .filtering = Texture::Filtering::Nearest
    }});

    AnimatedSprite sprite{{
      .frame = {
        .texture = texture,
      },
      .renderer = quadRenderer,
      .camera = camera
    }};

    CHECK(sprite.uuid().empty());

    sprite.instantiate();

    CHECK(!sprite.uuid().empty());
  }

  SUBCASE("Set Animation tests")
  {
    std::shared_ptr<QuadRenderer> quadRenderer = std::make_shared<QuadRenderer>(QuadRenderer{});
    std::shared_ptr<Camera> camera = std::make_shared<Camera>(Camera());
    std::shared_ptr<Texture> texture = std::make_shared<Texture>(Texture{{
      .image = dataPath + "/textures/animated_ball.png",
      .filtering = Texture::Filtering::Nearest
    }});

    AnimatedSprite sprite{{
      .frame = {
        .texture = texture,
        .index = 1,
        .size = glm::vec2(32.0f)
      },
      .transform = {
        .size = glm::vec2(128.0f)
      },
      .renderer = quadRenderer,
      .camera = camera,
    }};

    sprite.setAnimation({
      .name = "animation_1",
      .duration = 0.0f,
      .frames = {1, 2},
      .repeat = true
    });

    // Should contain the new animation
    CHECK(sprite.animations().find("animation_1") != sprite.animations().end());

    sprite.setAnimation({
      .name = "animation_1",
      .duration = 123.0f,
      .frames = {1},
      .repeat = false
    });

    auto animation = sprite.animations().at("animation_1");

    // Should update the existing animation
    CHECK(!animation.repeat);
  }

  SUBCASE("Play tests")
  {
    std::shared_ptr<QuadRenderer> quadRenderer = std::make_shared<QuadRenderer>(QuadRenderer{});
    std::shared_ptr<Camera> camera = std::make_shared<Camera>(Camera());
    std::shared_ptr<Texture> texture = std::make_shared<Texture>(Texture{{
      .image = dataPath + "/textures/animated_ball.png",
      .filtering = Texture::Filtering::Nearest
    }});

    AnimatedSprite sprite{{
      .frame = {
        .texture = texture,
        .index = 1,
        .size = glm::vec2(32.0f)
      },
      .transform = {
        .size = glm::vec2(128.0f)
      },
      .renderer = quadRenderer,
      .camera = camera,
    }};

    sprite.setAnimation({
      .name = "animation_1",
      .duration = 0.0f,
      .frames = {1, 2},
      .repeat = true
    });

    sprite.instantiate();
    sprite.play("animation_1");

    CHECK(sprite.playing());
    CHECK(sprite.currentAnimation() == "animation_1");
    CHECK(sprite.frame.index() == 1);
  }

  SUBCASE("Update/animate tests")
  {
    std::shared_ptr<QuadRenderer> quadRenderer = std::make_shared<QuadRenderer>(QuadRenderer{});
    std::shared_ptr<Camera> camera = std::make_shared<Camera>(Camera());
    std::shared_ptr<Texture> texture = std::make_shared<Texture>(Texture{{
      .image = dataPath + "/textures/animated_ball.png",
      .filtering = Texture::Filtering::Nearest
    }});

    AnimatedSprite sprite{{
      .frame = {
        .texture = texture,
        .index = 1,
        .size = glm::vec2(32.0f)
      },
      .transform = {
        .size = glm::vec2(128.0f)
      },
      .renderer = quadRenderer,
      .camera = camera,
    }};

    sprite.setAnimation({
      .name = "animation_1",
      .duration = 0.0f,
      .frames = {1, 2, 3},
      .repeat = true
    });

    sprite.instantiate();
    sprite.play("animation_1");

    sprite.update();

    CHECK(sprite.frame.index() == 2);

    sprite.update();

    CHECK(sprite.frame.index() == 3);

    sprite.update();
    sprite.update();

    CHECK(sprite.frame.index() == 2);

    sprite.setAnimation({
      .name = "animation_1",
      .duration = 0.0f,
      .frames = {1, 2, 3},
      .repeat = false
    });

    for(size_t i = 0; i < 10; i++) sprite.update();

    CHECK(sprite.frame.index() == 1);
  }

  SUBCASE("Stop tests")
  {
    std::shared_ptr<QuadRenderer> quadRenderer = std::make_shared<QuadRenderer>(QuadRenderer{});
    std::shared_ptr<Camera> camera = std::make_shared<Camera>(Camera());
    std::shared_ptr<Texture> texture = std::make_shared<Texture>(Texture{{
      .image = dataPath + "/textures/animated_ball.png",
      .filtering = Texture::Filtering::Nearest
    }});

    AnimatedSprite sprite{{
      .frame = {
        .texture = texture,
        .index = 1,
        .size = glm::vec2(32.0f)
      },
      .transform = {
        .size = glm::vec2(128.0f)
      },
      .renderer = quadRenderer,
      .camera = camera,
    }};

    sprite.setAnimation({
      .name = "animation_1",
      .duration = 0.0f,
      .frames = {1, 2, 3},
      .repeat = true
    });

    sprite.instantiate();
    sprite.play("animation_1");

    sprite.update();

    CHECK(sprite.frame.index() == 2);

    sprite.stop();

    CHECK(sprite.frame.index() == 1);

    CHECK(!sprite.playing());
  }
}