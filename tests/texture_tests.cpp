#include <catch2/catch_test_macros.hpp>
#include <ngyn/resources/texture.hpp>
#include <ngyn/util/files.hpp>
#include <ngyn/core/window.hpp>
#include <ngyn/util/logger.hpp>

using namespace ngyn;

// For GL initialization
Window window(WindowCreateInfo{});

TEST_CASE("Initialization", "[texture]")
{
  logger.setLevel(LoggerLevel::Disabled);

  SECTION("Handle should be equal GLuint max value if image doesn't exist")
  {
    Texture texture({.image = "invalid.png"});
    REQUIRE(texture.handle == std::numeric_limits<GLuint>::max());
  }

  SECTION("Handle should be equal GLuint max value if image exists but is invalid")
  {
    files::write("invalid.txt", "Text", {.recursive = true});

    Texture texture({.image = "invalid.txt"});
    REQUIRE(texture.handle == std::numeric_limits<GLuint>::max());
  }

  SECTION("Handle should be different from GLuint max value if texture is create correctly")
  {
    Texture texture({.image = "data/textures/idle.png"});
    REQUIRE(texture.handle != std::numeric_limits<GLuint>::max());
  }

  SECTION("Width and height should match image width and height")
  {
    int width = 320;
    int height = 32;

    Texture texture({.image = "data/textures/idle.png"});
    REQUIRE(texture.width == width);
    REQUIRE(texture.height == height);
  }

  SECTION("Handle shoul be equal to GLuint max value if destroyed")
  {
    Texture texture({.image = "data/textures/idle.png"});

    REQUIRE(texture.handle != std::numeric_limits<GLuint>::max());

    texture.destroy();

    REQUIRE(texture.handle == std::numeric_limits<GLuint>::max());
  }
}

TEST_CASE("Texture index", "[texture]")
{
  SECTION("Texture index should increment correctly")
  {
    int lastIndex;
    for(size_t i = 0; i < 2; i++)
    {
      lastIndex = Texture({.image = "data/textures/idle.png"}).index;
    }

    Texture texture({.image = "data/textures/idle.png"});
    REQUIRE(texture.index == lastIndex + 1);
  }

  SECTION("Texture index should be reused after destroying")
  {
    Texture texture1({.image = "data/textures/idle.png"});
    Texture texture2({.image = "data/textures/idle.png"});

    int reusedIndex = texture1.index;
    texture1.destroy();

    Texture texture({.image = "data/textures/idle.png"});

    REQUIRE(texture.index == reusedIndex);
  }

  SECTION("Texture index sholud be equal to GLuint max value if there are no more textures available")
  {
    for(size_t i = 0; i < 32; i++)
    {
      Texture({.image = "data/textures/idle.png"});
    }

    Texture texture({.image = "data/textures/idle.png"});

    REQUIRE(texture.index == std::numeric_limits<GLuint>::max());
  }
}