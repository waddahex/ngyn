#define DOCTEST_CONFIG_IMPLEMENT
#include <doctest/doctest.h>

#include <ngyn/ngyn.hpp>

using namespace ngyn;

std::string dataPath = "data";

Window window{{}};

int main(int argc, char **argv)
{
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

TEST_CASE("Initialization")
{
  Texture t({.image = dataPath + "/textures/arrows.png"});
  std::shared_ptr<Texture> texture = std::make_shared<Texture>(t);

  SUBCASE("Should have normal texCoords")
  {
    Frame frame{{
      .texture = texture,
      .flip = glm::bvec2(false, false)
    }};

    CHECK(frame.texCoords1() == glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    CHECK(frame.texCoords2() == glm::vec4(1.0f, 1.0f, 1.0f, 0.0f));
  }

  SUBCASE("Should have texCoords flipped on x axis")
  {
    Frame frame{{
      .texture = texture,
      .flip = glm::bvec2(true, false)
    }};

    CHECK(frame.texCoords1() == glm::vec4(1.0f, 1.0f, 1.0f, 0.0f));
    CHECK(frame.texCoords2() == glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
  }

  SUBCASE("Should have texCoords flipped on y axis")
  {
    Frame frame{{
      .texture = texture,
      .flip = glm::bvec2(false, true)
    }};

    CHECK(frame.texCoords1() == glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
    CHECK(frame.texCoords2() == glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));
  }

  SUBCASE("Should have texCoords flipped on y and x axes")
  {
    Frame frame{{
      .texture = texture,
      .flip = glm::bvec2(true, true)
    }};

    CHECK(frame.texCoords1() == glm::vec4(1.0f, 1.0f, 1.0f, 0.0f));
    CHECK(frame.texCoords2() == glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
  }
}

TEST_CASE("Update methods")
{
  Texture t({.image = dataPath + "/textures/arrows.png"});
  std::shared_ptr<Texture> texture = std::make_shared<Texture>(t);

  SUBCASE("Should update flip and texCoords")
  {
    Frame frame{{
      .texture = texture,
      .offset = glm::vec2(0.0f),
    }};

    glm::vec2 offset = frame.offset();
    glm::vec4 texCoords1 = frame.texCoords1();
    glm::vec4 texCoords2 = frame.texCoords2();

    frame.setOffset(glm::vec2(0.5f));

    CHECK(offset != frame.offset());
    CHECK(texCoords1 != frame.texCoords1());
    CHECK(texCoords2 != frame.texCoords2());
  }

  SUBCASE("Should update size and texCoords")
  {
    Frame frame{{
      .texture = texture,
      .size = glm::vec2(32.0f)
    }};

    glm::vec2 size = frame.size();
    glm::vec4 texCoords1 = frame.texCoords1();
    glm::vec4 texCoords2 = frame.texCoords2();

    frame.setSize(glm::vec2(16.0f));

    CHECK(size != frame.size());
    CHECK(texCoords1 != frame.texCoords1());
    CHECK(texCoords2 != frame.texCoords2());
  }

  SUBCASE("Should update flip and texCoords")
  {
    Frame frame{{
      .texture = texture,
      .flip = glm::bvec2(false, false)
    }};

    glm::bvec2 flip = frame.flip();
    glm::vec4 texCoords1 = frame.texCoords1();
    glm::vec4 texCoords2 = frame.texCoords2();

    frame.setFlip(glm::bvec2(true, false));

    CHECK(flip != frame.flip());
    CHECK(texCoords1 != frame.texCoords1());
    CHECK(texCoords2 != frame.texCoords2());
  }
}

TEST_CASE("Cut frame from texture")
{
  Texture t({.image = dataPath + "/textures/arrows.png"});
  std::shared_ptr<Texture> texture = std::make_shared<Texture>(t);

  SUBCASE("Should have the correct coordinates for offset.x different than 0")
  {
    Frame frame{{
      .texture = texture,
      .offset = glm::vec2(32.0f, 0.0f),
      .size = glm::vec2(32.0f),
      .flip = glm::bvec2(false, false)
    }};
    
    CHECK(frame.texCoords1() == glm::vec4(0.25, 0, 0.25, 0.25));
    CHECK(frame.texCoords2() == glm::vec4(0.5, 0.25, 0.5, 0));

    frame.setFlip(glm::bvec2(true, false));

    CHECK(frame.texCoords1() == glm::vec4(0.5, 0.25, 0.5, 0));
    CHECK(frame.texCoords2() == glm::vec4(0.25, 0, 0.25, 0.25));

    frame.setFlip(glm::bvec2(false, true));

    CHECK(frame.texCoords1() == glm::vec4(0.25, 0.25, 0.25, 0));
    CHECK(frame.texCoords2() == glm::vec4(0.5, 0, 0.5, 0.25));

    frame.setFlip(glm::bvec2(true, true));

    CHECK(frame.texCoords1() == glm::vec4(0.5, 0.25, 0.5, 0));
    CHECK(frame.texCoords2() == glm::vec4(0.25, 0, 0.25, 0.25));
  }

  SUBCASE("Should have the correct coordinates for offset.y different than 0")
  {
    Frame frame{{
      .texture = texture,
      .offset = glm::vec2(0.0f, 32.0f),
      .size = glm::vec2(32.0f),
      .flip = glm::bvec2(false, false)
    }};

    CHECK(frame.texCoords1() == glm::vec4(0, 0.25, 0, 0.5));
    CHECK(frame.texCoords2() == glm::vec4(0.25, 0.5, 0.25, 0.25));

    frame.setFlip(glm::bvec2(true, false));

    CHECK(frame.texCoords1() == glm::vec4(0.25, 0.5, 0.25, 0.25));
    CHECK(frame.texCoords2() == glm::vec4(0, 0.25, 0, 0.5));

    frame.setFlip(glm::bvec2(false, true));

    CHECK(frame.texCoords1() == glm::vec4(0, 0.5, 0, 0.25));
    CHECK(frame.texCoords2() == glm::vec4(0.25, 0.25, 0.25, 0.5));

    frame.setFlip(glm::bvec2(true, true));

    CHECK(frame.texCoords1() == glm::vec4(0.25, 0.5, 0.25, 0.25));
    CHECK(frame.texCoords2() == glm::vec4(0, 0.25, 0, 0.5));
  }

  SUBCASE("Should have the correct coordinates for offset different than (0, 0)")
  {
    Frame frame{{
      .texture = texture,
      .offset = glm::vec2(32.0f),
      .size = glm::vec2(32.0f),
      .flip = glm::bvec2(false, false)
    }};

    CHECK(frame.texCoords1() == glm::vec4(0.25, 0.25, 0.25, 0.5));
    CHECK(frame.texCoords2() == glm::vec4(0.5, 0.5, 0.5, 0.25));

    frame.setFlip(glm::bvec2(true, false));

    CHECK(frame.texCoords1() == glm::vec4(0.5, 0.5, 0.5, 0.25));
    CHECK(frame.texCoords2() == glm::vec4(0.25, 0.25, 0.25, 0.5));

    frame.setFlip(glm::bvec2(false, true));

    CHECK(frame.texCoords1() == glm::vec4(0.25, 0.5, 0.25, 0.25));
    CHECK(frame.texCoords2() == glm::vec4(0.5, 0.25, 0.5, 0.5));

    frame.setFlip(glm::bvec2(true, true));

    CHECK(frame.texCoords1() == glm::vec4(0.5, 0.5, 0.5, 0.25));
    CHECK(frame.texCoords2() == glm::vec4(0.25, 0.25, 0.25, 0.5));
  }
}

TEST_CASE("Cut frame from texture using frame index")
{
  Texture t({.image = dataPath + "/textures/arrows.png"});
  std::shared_ptr<Texture> texture = std::make_shared<Texture>(t);

  SUBCASE("Should have the correct coordinates for offset.x different than 0")
  {
    Frame frame{{
      .texture = texture,
      .index = 2,
      // .offset = glm::vec2(32.0f, 0.0f),
      .size = glm::vec2(32.0f),
      .flip = glm::bvec2(false, false)
    }};
    
    CHECK(frame.texCoords1() == glm::vec4(0.25, 0, 0.25, 0.25));
    CHECK(frame.texCoords2() == glm::vec4(0.5, 0.25, 0.5, 0));

    frame.setFlip(glm::bvec2(true, false));

    CHECK(frame.texCoords1() == glm::vec4(0.5, 0.25, 0.5, 0));
    CHECK(frame.texCoords2() == glm::vec4(0.25, 0, 0.25, 0.25));

    frame.setFlip(glm::bvec2(false, true));

    CHECK(frame.texCoords1() == glm::vec4(0.25, 0.25, 0.25, 0));
    CHECK(frame.texCoords2() == glm::vec4(0.5, 0, 0.5, 0.25));

    frame.setFlip(glm::bvec2(true, true));

    CHECK(frame.texCoords1() == glm::vec4(0.5, 0.25, 0.5, 0));
    CHECK(frame.texCoords2() == glm::vec4(0.25, 0, 0.25, 0.25));
  }

  SUBCASE("Should have the correct coordinates for offset.y different than 0")
  {
    Frame frame{{
      .texture = texture,
      .index = 5,
      // .offset = glm::vec2(0.0f, 32.0f),
      .size = glm::vec2(32.0f),
      .flip = glm::bvec2(false, false)
    }};

    CHECK(frame.texCoords1() == glm::vec4(0, 0.25, 0, 0.5));
    CHECK(frame.texCoords2() == glm::vec4(0.25, 0.5, 0.25, 0.25));

    frame.setFlip(glm::bvec2(true, false));

    CHECK(frame.texCoords1() == glm::vec4(0.25, 0.5, 0.25, 0.25));
    CHECK(frame.texCoords2() == glm::vec4(0, 0.25, 0, 0.5));

    frame.setFlip(glm::bvec2(false, true));

    CHECK(frame.texCoords1() == glm::vec4(0, 0.5, 0, 0.25));
    CHECK(frame.texCoords2() == glm::vec4(0.25, 0.25, 0.25, 0.5));

    frame.setFlip(glm::bvec2(true, true));

    CHECK(frame.texCoords1() == glm::vec4(0.25, 0.5, 0.25, 0.25));
    CHECK(frame.texCoords2() == glm::vec4(0, 0.25, 0, 0.5));
  }

  SUBCASE("Should have the correct coordinates for offset different than (0, 0)")
  {
    Frame frame{{
      .texture = texture,
      .index = 6,
      // .offset = glm::vec2(32.0f),
      .size = glm::vec2(32.0f),
      .flip = glm::bvec2(false, false)
    }};

    CHECK(frame.texCoords1() == glm::vec4(0.25, 0.25, 0.25, 0.5));
    CHECK(frame.texCoords2() == glm::vec4(0.5, 0.5, 0.5, 0.25));

    frame.setFlip(glm::bvec2(true, false));

    CHECK(frame.texCoords1() == glm::vec4(0.5, 0.5, 0.5, 0.25));
    CHECK(frame.texCoords2() == glm::vec4(0.25, 0.25, 0.25, 0.5));

    frame.setFlip(glm::bvec2(false, true));

    CHECK(frame.texCoords1() == glm::vec4(0.25, 0.5, 0.25, 0.25));
    CHECK(frame.texCoords2() == glm::vec4(0.5, 0.25, 0.5, 0.5));

    frame.setFlip(glm::bvec2(true, true));

    CHECK(frame.texCoords1() == glm::vec4(0.5, 0.5, 0.5, 0.25));
    CHECK(frame.texCoords2() == glm::vec4(0.25, 0.25, 0.25, 0.5));
  }
}