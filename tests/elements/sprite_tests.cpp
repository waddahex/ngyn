#define DOCTEST_CONFIG_IMPLEMENT
#include <doctest/doctest.h>

#include <ngyn/ngyn.hpp>

using namespace ngyn;

std::string dataPath = "data";

int main(int argc, char **argv)
{
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

static Window window{{}};

TEST_CASE("General methods")
{
  SUBCASE("Instantiate should set instanceIndex >= 0")
  {
    std::shared_ptr<QuadRenderer> quadRenderer = std::make_shared<QuadRenderer>(QuadRenderer{});
    std::shared_ptr<Camera> camera = std::make_shared<Camera>(Camera());
    std::shared_ptr<Texture> texture = std::make_shared<Texture>(Texture{{
      .image = dataPath + "/textures/arrows.png",
      .filtering = Texture::Filtering::Nearest
    }});

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

    CHECK(sprite.instanceIndex() >= 0);
  }

  SUBCASE("Update should change the data on the renderer")
  {
    std::shared_ptr<QuadRenderer> quadRenderer = std::make_shared<QuadRenderer>(QuadRenderer{});
    std::shared_ptr<Camera> camera = std::make_shared<Camera>(Camera());
    std::shared_ptr<Texture> texture = std::make_shared<Texture>(Texture{{
      .image = dataPath + "/textures/arrows.png",
      .filtering = Texture::Filtering::Nearest
    }});

    Sprite sprite{{
      .frame = {
        .texture = texture,
        .offset = glm::vec2(0.0f),
        .size = glm::vec2(32.0f),
        .color = Color(255)
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

    sprite.frame.setColor(Color(123));
    sprite.update();

    auto newData = quadRenderer.get()->getInstance(sprite.instanceIndex());

    CHECK(data.color != newData.color);
  }
}

TEST_CASE("Increments")
{
  SUBCASE("Position should increase by provided velocity and model should be updated")
  {
    std::shared_ptr<QuadRenderer> quadRenderer = std::make_shared<QuadRenderer>(QuadRenderer{});
    std::shared_ptr<Camera> camera = std::make_shared<Camera>(Camera());
    std::shared_ptr<Texture> texture = std::make_shared<Texture>(Texture{{
      .image = dataPath + "/textures/arrows.png",
      .filtering = Texture::Filtering::Nearest
    }});

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

    CHECK(sprite.transform.position() == position + velocity);
    CHECK(sprite.transform.model() != model);
  }

  SUBCASE("Size should increase by provided size and model should be updated")
  {
    std::shared_ptr<QuadRenderer> quadRenderer = std::make_shared<QuadRenderer>(QuadRenderer{});
    std::shared_ptr<Camera> camera = std::make_shared<Camera>(Camera());
    std::shared_ptr<Texture> texture = std::make_shared<Texture>(Texture{{
      .image = dataPath + "/textures/arrows.png",
      .filtering = Texture::Filtering::Nearest
    }});

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

    CHECK(sprite.transform.size() == size + extraSize);
    CHECK(sprite.transform.model() != model);
  }

  SUBCASE("Scale should increase by provided extraScale and model should be updated")
  {
    std::shared_ptr<QuadRenderer> quadRenderer = std::make_shared<QuadRenderer>(QuadRenderer{});
    std::shared_ptr<Camera> camera = std::make_shared<Camera>(Camera());
    std::shared_ptr<Texture> texture = std::make_shared<Texture>(Texture{{
      .image = dataPath + "/textures/arrows.png",
      .filtering = Texture::Filtering::Nearest
    }});

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

    CHECK(sprite.transform.scale() == scale + extraScale);
    CHECK(sprite.transform.model() != model);
  }

  SUBCASE("Rotation should increase by provided degrees and model should be updated")
  {
    std::shared_ptr<QuadRenderer> quadRenderer = std::make_shared<QuadRenderer>(QuadRenderer{});
    std::shared_ptr<Camera> camera = std::make_shared<Camera>(Camera());
    std::shared_ptr<Texture> texture = std::make_shared<Texture>(Texture{{
      .image = dataPath + "/textures/arrows.png",
      .filtering = Texture::Filtering::Nearest
    }});

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

    CHECK(sprite.transform.rotation() == rotation + degrees);
    CHECK(sprite.transform.model() != model);
  }
}