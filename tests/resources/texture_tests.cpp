#define DOCTEST_CONFIG_IMPLEMENT
#include <doctest/doctest.h>

#include <ngyn/ngyn.hpp>

#include <stb_image.h>

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

TEST_CASE("Initialization")
{
  Window window{{}};
  logger.setLevel(LoggerLevel::Disabled);

  SUBCASE("Texture should be valid when creating it from image file")
  {
    Texture texture({.image = dataPath + "/textures/arrows.png"});
    REQUIRE(texture.isValid());
  }

  SUBCASE("Texture should be valid when creating it from data and size")
  {
    glm::ivec2 size;
    int channels;

    auto data = stbi_load(std::string(dataPath + "/textures/arrows.png").c_str(), &size.x, &size.y, &channels, 0);

    Texture texture({.size = size, .data = data});

    stbi_image_free(data);

    REQUIRE(texture.isValid());
  }

  SUBCASE("Texture should be valid when creating it from image file")
  {
    Texture texture({.image = dataPath + "/textures/arrows.png"});
    REQUIRE(texture.isValid());
  }

  SUBCASE("Width and height should match image width and height")
  {
    glm::ivec2 size(128);

    Texture texture({.image = dataPath + "/textures/arrows.png"});

    REQUIRE(texture.size() == size);
  }

  SUBCASE("Texture should be invalid after destroying")
  {
    Texture texture({.image = dataPath + "/textures/arrows.png"});

    texture.destroy();

    REQUIRE(!texture.isValid());
  }
}