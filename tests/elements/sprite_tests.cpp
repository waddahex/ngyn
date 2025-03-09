#include <catch2/catch_test_macros.hpp>
#include <ngyn/ngyn.hpp>

using namespace ngyn;

TEST_CASE("General methods", "[sprite]")
{
  SECTION("Instantiate should set instanceIndex >= 0")
  {
    std::shared_ptr<QuadRenderer> quadRenderer = std::make_shared<QuadRenderer>(QuadRenderer{});
    std::shared_ptr<Camera> camera = std::make_shared<Camera>(Camera());
    std::shared_ptr<Texture> texture = std::make_shared<Texture>(Texture());

    Sprite sprite{{
      .frame = {
        .texture = texture,
        .offset = glm::vec2(0.0f),
        .size = glm::vec2(32.0f)
      },
      .transform = {
        .position = glm::vec2(0.0f),
        .size = glm::vec2(128.0f)
      },
      .renderer = quadRenderer,
      .camera = camera
    }};

    sprite.instantiate();

    REQUIRE(sprite.instanceIndex() >= 0);
  }

  SECTION("Update should change the data on the renderer")
  {
    std::shared_ptr<QuadRenderer> quadRenderer = std::make_shared<QuadRenderer>(QuadRenderer{});
    std::shared_ptr<Camera> camera = std::make_shared<Camera>(Camera());
    std::shared_ptr<Texture> texture = std::make_shared<Texture>(Texture());

    Sprite sprite{{
      .frame = {
        .texture = texture,
        .offset = glm::vec2(0.0f),
        .size = glm::vec2(32.0f),
        .color = glm::vec4(1.0f)
      },
      .transform = {
        .position = glm::vec2(0.0f),
        .size = glm::vec2(128.0f)
      },
      .renderer = quadRenderer,
      .camera = camera
    }};

    sprite.instantiate();

    auto data = quadRenderer.get()->getInstance(sprite.instanceIndex());

    sprite.frame.setColor(glm::vec4(2.0f));
    sprite.update();

    auto newData = quadRenderer.get()->getInstance(sprite.instanceIndex());

    REQUIRE(data.color != newData.color);
  }
}

TEST_CASE("Increments", "[sprite]")
{
  SECTION("Position should increase by provided velocity and model should be updated")
  {
    std::shared_ptr<QuadRenderer> quadRenderer = std::make_shared<QuadRenderer>(QuadRenderer{});
    std::shared_ptr<Camera> camera = std::make_shared<Camera>(Camera());
    std::shared_ptr<Texture> texture = std::make_shared<Texture>(Texture());

    Sprite sprite{{
      .frame = {
        .texture = texture,
        .offset = glm::vec2(0.0f),
        .size = glm::vec2(32.0f)
      },
      .transform = {
        .position = glm::vec2(0.0f),
        .size = glm::vec2(128.0f)
      },
      .renderer = quadRenderer,
      .camera = camera
    }};

    auto model = sprite.transform.model();
    auto position = sprite.transform.position();

    glm::vec2 velocity(1.0f);
    sprite.moveBy(velocity);

    REQUIRE(sprite.transform.position() == position + velocity);
    REQUIRE(sprite.transform.model() != model);
  }

  SECTION("Size should increase by provided size and model should be updated")
  {
    std::shared_ptr<QuadRenderer> quadRenderer = std::make_shared<QuadRenderer>(QuadRenderer{});
    std::shared_ptr<Camera> camera = std::make_shared<Camera>(Camera());
    std::shared_ptr<Texture> texture = std::make_shared<Texture>(Texture());

    Sprite sprite{{
      .frame = {
        .texture = texture,
        .offset = glm::vec2(0.0f),
        .size = glm::vec2(32.0f)
      },
      .transform = {
        .position = glm::vec2(0.0f),
        .size = glm::vec2(128.0f)
      },
      .renderer = quadRenderer,
      .camera = camera
    }};

    auto model = sprite.transform.model();
    auto size = sprite.transform.size();

    glm::vec2 extraSize(1.0f);
    sprite.resizeBy(extraSize);

    REQUIRE(sprite.transform.size() == size + extraSize);
    REQUIRE(sprite.transform.model() != model);
  }

  SECTION("Scale should increase by provided extraScale and model should be updated")
  {
    std::shared_ptr<QuadRenderer> quadRenderer = std::make_shared<QuadRenderer>(QuadRenderer{});
    std::shared_ptr<Camera> camera = std::make_shared<Camera>(Camera());
    std::shared_ptr<Texture> texture = std::make_shared<Texture>(Texture());

    Sprite sprite{{
      .frame = {
        .texture = texture,
        .offset = glm::vec2(0.0f),
        .size = glm::vec2(32.0f)
      },
      .transform = {
        .position = glm::vec2(0.0f),
        .size = glm::vec2(128.0f)
      },
      .renderer = quadRenderer,
      .camera = camera
    }};

    auto model = sprite.transform.model();
    auto scale = sprite.transform.scale();

    glm::vec2 extraScale(1.0f);
    sprite.scaleBy(extraScale);

    REQUIRE(sprite.transform.scale() == scale + extraScale);
    REQUIRE(sprite.transform.model() != model);
  }

  SECTION("Rotation should increase by provided degrees and model should be updated")
  {
    std::shared_ptr<QuadRenderer> quadRenderer = std::make_shared<QuadRenderer>(QuadRenderer{});
    std::shared_ptr<Camera> camera = std::make_shared<Camera>(Camera());
    std::shared_ptr<Texture> texture = std::make_shared<Texture>(Texture());

    Sprite sprite{{
      .frame = {
        .texture = texture,
        .offset = glm::vec2(0.0f),
        .size = glm::vec2(32.0f)
      },
      .transform = {
        .position = glm::vec2(0.0f),
        .size = glm::vec2(128.0f)
      },
      .renderer = quadRenderer,
      .camera = camera
    }};

    auto model = sprite.transform.model();
    auto rotation = sprite.transform.rotation();

    float degrees = 10.0f;
    sprite.rotateBy(degrees);

    REQUIRE(sprite.transform.rotation() == rotation + degrees);
    REQUIRE(sprite.transform.model() != model);
  }
}