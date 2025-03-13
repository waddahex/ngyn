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

TEST_CASE("Font tests")
{
  SUBCASE("Font should be valid")
  {
    Font font{{
      .path = dataPath + "/fonts/arial.ttf",
      .name = "valid",
      .size = 16
    }};

    CHECK(font.isValid());
  }

  SUBCASE("Characters should not be empty")
  {
    Font font{{
      .path = dataPath + "/fonts/arial.ttf",
      .name = "arial",
      .size = 16
    }};

    CHECK(!font.characters().empty());
  }

  SUBCASE("Should use file path as name")
  {
    Font font{{
      .path = dataPath + "/fonts/arial.ttf",
      .size = 16
    }};

    CHECK(font.name() == dataPath + "/fonts/arial.ttf");
  }

  SUBCASE("Texture should be created")
  {
    Font font{{
      .path = dataPath + "/fonts/arial.ttf",
      .name = "arial",
      .size = 18
    }};

    auto texture = ResourcesManager::getResource<Texture>("arial_18");

    CHECK(font.texture().lock());
    CHECK(texture.lock());
  }

  SUBCASE("maxHeight should be equal texture.size.y")
  {
    Font font{{
      .path = dataPath + "/fonts/arial.ttf",
      .name = "maxHeightArial",
      .size = 18
    }};

    auto texture = ResourcesManager::getResource<Texture>("maxHeightArial_18");
    auto size = texture.lock().get()->size();

    CHECK(size.y == font.maxHeight());
  }

  SUBCASE("Should be invalid after destryoing")
  {
    Font font{{
      .path = dataPath + "/fonts/arial.ttf",
      .name = "destroyed",
      .size = 18
    }};

    font.destroy();

    CHECK(!font.isValid());
  }

  SUBCASE("Should invalidate and remove texture pointer from resourcesManager")
  {
    Font font{{
      .path = dataPath + "/fonts/arial.ttf",
      .name = "removedArial",
      .size = 18
    }};

    font.destroy();

    auto storage = ResourcesManager::getStorage<Font>();

    CHECK(!font.texture().lock());
    CHECK(storage.resources.find("removedArial_18") == storage.resources.end());
  }
}