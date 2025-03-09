#include <catch2/catch_test_macros.hpp>
#include <ngyn/ngyn.hpp>

using namespace ngyn;

TEST_CASE("RGBA", "[color]")
{
  SECTION("Should be equal (1.0f, 1.0f, 1.0f, 1.0f) passing only 1 int parameters")
  {
    Color color(255);

    REQUIRE(color == glm::vec4(1.0f));
  }

  SECTION("Should be equal (1.0f, 1.0f, 1.0f, 1.0f) passing only 3 int parameters")
  {
    Color color(255, 255, 255);

    REQUIRE(color == glm::vec4(1.0f));
  }

  SECTION("Should be equal (1.0f, 1.0f, 1.0f, 1.0f) passing only 4 int parameters")
  {
    Color color(255, 255, 255, 255);

    REQUIRE(color == glm::vec4(1.0f));
  }
}

TEST_CASE("Hex string color", "[color]")
{
  SECTION("Shoulg be equal (1.0f, 1.0f, 1.0f, 1.0f) passing #RGB")
  {
    Color color("#FFF");

    REQUIRE(color == glm::vec4(1.0f));
  }

  SECTION("Shoulg be equal (1.0f, 1.0f, 1.0f, 1.0f) passing #RGBA")
  {
    Color color("#FFFF");

    REQUIRE(color == glm::vec4(1.0f));
  }

  SECTION("Shoulg be equal (1.0f, 1.0f, 1.0f, 1.0f) passing #RRGGBB")
  {
    Color color("#FFFFFF");

    REQUIRE(color == glm::vec4(1.0f));
  }

  SECTION("Shoulg be equal (1.0f, 1.0f, 1.0f, 1.0f) passing #RRGGBBAA")
  {
    Color color("#FFFFFFFF");

    REQUIRE(color == glm::vec4(1.0f));
  }
}