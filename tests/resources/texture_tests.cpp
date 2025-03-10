#include <catch2/catch_test_macros.hpp>
#include <ngyn/ngyn.hpp>
#include <stb_image.h>

using namespace ngyn;

TEST_CASE("Initialization", "[texture]")
{
  // For GL initialization
  Window window(WindowCreateInfo{});
  // logger.setLevel(LoggerLevel::Disabled);

  SECTION("Texture should be valid when creating it from image file")
  {
    Texture texture({.image = "data/textures/arrows.png"});
    REQUIRE(texture.isValid());
  }

  SECTION("Texture should be valid when creating it from data and size")
  {
    glm::ivec2 size;
    int channels;

    auto data = stbi_load("data/textures/arrows.png", &size.x, &size.y, &channels, 0);

    Texture texture({.size = size, .data = data});

    stbi_image_free(data);

    REQUIRE(texture.isValid());
  }

  SECTION("Texture should be valid when creating it from image file")
  {
    Texture texture({.image = "data/textures/arrows.png"});
    REQUIRE(texture.isValid());
  }

  SECTION("Width and height should match image width and height")
  {
    glm::ivec2 size(128);

    Texture texture({.image = "data/textures/arrows.png"});

    REQUIRE(texture.size() == size);
  }

  SECTION("Texture should be invalid after destroying")
  {
    Texture texture({.image = "data/textures/arrows.png"});

    texture.destroy();

    REQUIRE(!texture.isValid());
  }
}