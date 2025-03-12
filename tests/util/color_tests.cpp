#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <ngyn/ngyn.hpp>

using namespace ngyn;

TEST_CASE("RGBA")
{
  SUBCASE("Should be equal (1.0f, 1.0f, 1.0f, 1.0f) passing only 1 int parameters")
  {
    Color color(255);

    CHECK(color == glm::vec4(1.0f));
  }

  SUBCASE("Should be equal (1.0f, 1.0f, 1.0f, 1.0f) passing only 3 int parameters")
  {
    Color color(255, 255, 255);

    CHECK(color == glm::vec4(1.0f));
  }

  SUBCASE("Should be equal (1.0f, 1.0f, 1.0f, 1.0f) passing only 4 int parameters")
  {
    Color color(255, 255, 255, 255);

    CHECK(color == glm::vec4(1.0f));
  }
}

TEST_CASE("Hex string color")
{
  SUBCASE("Shoulg be equal (1.0f, 1.0f, 1.0f, 1.0f) passing #RGB")
  {
    Color color("#FFF");

    CHECK(color == glm::vec4(1.0f));
  }

  SUBCASE("Shoulg be equal (1.0f, 1.0f, 1.0f, 1.0f) passing #RGBA")
  {
    Color color("#FFFF");

    CHECK(color == glm::vec4(1.0f));
  }

  SUBCASE("Shoulg be equal (1.0f, 1.0f, 1.0f, 1.0f) passing #RRGGBB")
  {
    Color color("#FFFFFF");

    CHECK(color == glm::vec4(1.0f));
  }

  SUBCASE("Shoulg be equal (1.0f, 1.0f, 1.0f, 1.0f) passing #RRGGBBAA")
  {
    Color color("#FFFFFFFF");

    CHECK(color == glm::vec4(1.0f));
  }
}