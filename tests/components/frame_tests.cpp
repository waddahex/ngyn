#include <catch2/catch_test_macros.hpp>
#include <ngyn/ngyn.hpp>

using namespace ngyn;

TEST_CASE("Initialization", "[frame]")
{
  SECTION("Should have normal texCoords")
  {
    Texture t;
    t.width = 32;
    t.height = 32;

    std::shared_ptr<Texture> texture = std::make_shared<Texture>(t);

    Frame frame{{
      .texture = texture,
      .flip = glm::bvec2(false, false)
    }};

    REQUIRE(frame.texCoords1() == glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    REQUIRE(frame.texCoords2() == glm::vec4(1.0f, 1.0f, 1.0f, 0.0f));
  }

  SECTION("Should have texCoords flipped on x axis")
  {
    Texture t;
    t.width = 32;
    t.height = 32;

    std::shared_ptr<Texture> texture = std::make_shared<Texture>(t);

    Frame frame{{
      .texture = texture,
      .flip = glm::bvec2(true, false)
    }};

    REQUIRE(frame.texCoords1() == glm::vec4(1.0f, 1.0f, 1.0f, 0.0f));
    REQUIRE(frame.texCoords2() == glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
  }

  SECTION("Should have texCoords flipped on y axis")
  {
    Texture t;
    t.width = 32;
    t.height = 32;

    std::shared_ptr<Texture> texture = std::make_shared<Texture>(t);

    Frame frame{{
      .texture = texture,
      .flip = glm::bvec2(false, true)
    }};

    REQUIRE(frame.texCoords1() == glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
    REQUIRE(frame.texCoords2() == glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));
  }

  SECTION("Should have texCoords flipped on y and x axes")
  {
    Texture t;
    t.width = 32;
    t.height = 32;

    std::shared_ptr<Texture> texture = std::make_shared<Texture>(t);

    Frame frame{{
      .texture = texture,
      .flip = glm::bvec2(true, true)
    }};

    REQUIRE(frame.texCoords1() == glm::vec4(1.0f, 1.0f, 1.0f, 0.0f));
    REQUIRE(frame.texCoords2() == glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
  }
}

TEST_CASE("Update methods", "[frame]")
{
  // TODO: test texture after changing resource manager

  SECTION("Should update flip and texCoords")
  {
    Texture t;
    t.width = 32;
    t.height = 32;

    std::shared_ptr<Texture> texture = std::make_shared<Texture>(t);

    Frame frame{{
      .texture = texture,
      .offset = glm::vec2(0.0f),
    }};

    glm::vec2 offset = frame.offset();
    glm::vec4 texCoords1 = frame.texCoords1();
    glm::vec4 texCoords2 = frame.texCoords2();

    frame.setOffset(glm::vec2(0.5f));

    REQUIRE(offset != frame.offset());
    REQUIRE(texCoords1 != frame.texCoords1());
    REQUIRE(texCoords2 != frame.texCoords2());
  }

  SECTION("Should update size and texCoords")
  {
    Texture t;
    t.width = 32;
    t.height = 32;

    std::shared_ptr<Texture> texture = std::make_shared<Texture>(t);

    Frame frame{{
      .texture = texture,
      .size = glm::vec2(32.0f)
    }};

    glm::vec2 size = frame.size();
    glm::vec4 texCoords1 = frame.texCoords1();
    glm::vec4 texCoords2 = frame.texCoords2();

    frame.setSize(glm::vec2(16.0f));

    REQUIRE(size != frame.size());
    REQUIRE(texCoords1 != frame.texCoords1());
    REQUIRE(texCoords2 != frame.texCoords2());
  }

  SECTION("Should update flip and texCoords")
  {
    Texture t;
    t.width = 32;
    t.height = 32;

    std::shared_ptr<Texture> texture = std::make_shared<Texture>(t);

    Frame frame{{
      .texture = texture,
      .flip = glm::bvec2(false, false)
    }};

    glm::bvec2 flip = frame.flip();
    glm::vec4 texCoords1 = frame.texCoords1();
    glm::vec4 texCoords2 = frame.texCoords2();

    frame.setFlip(glm::bvec2(true, false));

    REQUIRE(flip != frame.flip());
    REQUIRE(texCoords1 != frame.texCoords1());
    REQUIRE(texCoords2 != frame.texCoords2());
  }
}

TEST_CASE("Cut frame from texture", "[frame]")
{
  SECTION("Should have the correct coordinates for offset.x different than 0")
  {
    Texture t;
    t.width = 128;
    t.height = 128;

    std::shared_ptr<Texture> texture = std::make_shared<Texture>(t);

    Frame frame{{
      .texture = texture,
      .offset = glm::vec2(32.0f, 0.0f),
      .size = glm::vec2(32.0f),
      .flip = glm::bvec2(false, false)
    }};
    
    REQUIRE(frame.texCoords1() == glm::vec4(0.25, 0, 0.25, 0.25));
    REQUIRE(frame.texCoords2() == glm::vec4(0.5, 0.25, 0.5, 0));

    frame.setFlip(glm::bvec2(true, false));

    REQUIRE(frame.texCoords1() == glm::vec4(0.5, 0.25, 0.5, 0));
    REQUIRE(frame.texCoords2() == glm::vec4(0.25, 0, 0.25, 0.25));

    frame.setFlip(glm::bvec2(false, true));

    REQUIRE(frame.texCoords1() == glm::vec4(0.25, 0.25, 0.25, 0));
    REQUIRE(frame.texCoords2() == glm::vec4(0.5, 0, 0.5, 0.25));

    frame.setFlip(glm::bvec2(true, true));

    REQUIRE(frame.texCoords1() == glm::vec4(0.5, 0.25, 0.5, 0));
    REQUIRE(frame.texCoords2() == glm::vec4(0.25, 0, 0.25, 0.25));
  }

  SECTION("Should have the correct coordinates for offset.y different than 0")
  {
    Texture t;
    t.width = 128;
    t.height = 128;

    std::shared_ptr<Texture> texture = std::make_shared<Texture>(t);

    Frame frame{{
      .texture = texture,
      .offset = glm::vec2(0.0f, 32.0f),
      .size = glm::vec2(32.0f),
      .flip = glm::bvec2(false, false)
    }};

    REQUIRE(frame.texCoords1() == glm::vec4(0, 0.25, 0, 0.5));
    REQUIRE(frame.texCoords2() == glm::vec4(0.25, 0.5, 0.25, 0.25));

    frame.setFlip(glm::bvec2(true, false));

    REQUIRE(frame.texCoords1() == glm::vec4(0.25, 0.5, 0.25, 0.25));
    REQUIRE(frame.texCoords2() == glm::vec4(0, 0.25, 0, 0.5));

    frame.setFlip(glm::bvec2(false, true));

    REQUIRE(frame.texCoords1() == glm::vec4(0, 0.5, 0, 0.25));
    REQUIRE(frame.texCoords2() == glm::vec4(0.25, 0.25, 0.25, 0.5));

    frame.setFlip(glm::bvec2(true, true));

    REQUIRE(frame.texCoords1() == glm::vec4(0.25, 0.5, 0.25, 0.25));
    REQUIRE(frame.texCoords2() == glm::vec4(0, 0.25, 0, 0.5));
  }

  SECTION("Should have the correct coordinates for offset different than (0, 0)")
  {
    Texture t;
    t.width = 128;
    t.height = 128;

    std::shared_ptr<Texture> texture = std::make_shared<Texture>(t);

    Frame frame{{
      .texture = texture,
      .offset = glm::vec2(32.0f),
      .size = glm::vec2(32.0f),
      .flip = glm::bvec2(false, false)
    }};

    REQUIRE(frame.texCoords1() == glm::vec4(0.25, 0.25, 0.25, 0.5));
    REQUIRE(frame.texCoords2() == glm::vec4(0.5, 0.5, 0.5, 0.25));

    frame.setFlip(glm::bvec2(true, false));

    REQUIRE(frame.texCoords1() == glm::vec4(0.5, 0.5, 0.5, 0.25));
    REQUIRE(frame.texCoords2() == glm::vec4(0.25, 0.25, 0.25, 0.5));

    frame.setFlip(glm::bvec2(false, true));

    REQUIRE(frame.texCoords1() == glm::vec4(0.25, 0.5, 0.25, 0.25));
    REQUIRE(frame.texCoords2() == glm::vec4(0.5, 0.25, 0.5, 0.5));

    frame.setFlip(glm::bvec2(true, true));

    REQUIRE(frame.texCoords1() == glm::vec4(0.5, 0.5, 0.5, 0.25));
    REQUIRE(frame.texCoords2() == glm::vec4(0.25, 0.25, 0.25, 0.5));
  }
}