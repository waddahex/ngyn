#include <catch2/catch_test_macros.hpp>
#include <ngyn/ngyn.hpp>

using namespace ngyn;

TEST_CASE("Font tests", "[font]")
{
  Window window(WindowCreateInfo{});

  SECTION("Font should be valid")
  {
    Font font{{
      .path = "data/fonts/arial.ttf",
      .name = "valid",
      .size = 16
    }};

    REQUIRE(font.isValid());
  }

  SECTION("Characters should not be empty")
  {
    Font font{{
      .path = "data/fonts/arial.ttf",
      .name = "arial",
      .size = 16
    }};

    REQUIRE(!font.characters().empty());
  }

  SECTION("Should use file path as name")
  {
    Font font{{
      .path = "data/fonts/arial.ttf",
      .size = 16
    }};

    REQUIRE(font.name() == "data/fonts/arial.ttf");
  }

  SECTION("Texture should be created")
  {
    Font font{{
      .path = "data/fonts/arial.ttf",
      .name = "arial",
      .size = 18
    }};

    auto texture = ResourcesManager::getResource<Texture>("arial_18");

    REQUIRE(font.texture().lock());
    REQUIRE(texture.lock());
  }

  SECTION("maxHeight should be equal texture.size.y")
  {
    Font font{{
      .path = "data/fonts/arial.ttf",
      .name = "maxHeightArial",
      .size = 18
    }};

    auto texture = ResourcesManager::getResource<Texture>("maxHeightArial_18");
    auto size = texture.lock().get()->size();

    REQUIRE(size.y == font.maxHeight());
  }

  SECTION("Should be invalid after destryoing")
  {
    Font font{{
      .path = "data/fonts/arial.ttf",
      .name = "destroyed",
      .size = 18
    }};

    font.destroy();

    REQUIRE(!font.isValid());
  }

  SECTION("Should invalidate and remove texture pointer from resourcesManager")
  {
    Font font{{
      .path = "data/fonts/arial.ttf",
      .name = "removedArial",
      .size = 18
    }};

    font.destroy();

    auto storage = ResourcesManager::getStorage<Font>();

    REQUIRE(!font.texture().lock());
    REQUIRE(storage.resources.find("removedArial_18") == storage.resources.end());
  }
}