#include <catch2/catch_test_macros.hpp>

#include <ngyn/util/random.hpp>

using namespace ngyn;

TEST_CASE("Max and min boundaries", "[random]")
{
  SECTION("Float value should be between max and min")
  {
    float min = 0.0f;
    float max = 10.0f;

    for(size_t i = 0; i < max; i++)
    {
      float randomFloat = random::getFloat(min, max);

      bool boundaryCheck = randomFloat >= min && randomFloat <= max;

      REQUIRE(boundaryCheck);
    }
  }

  SECTION("Int value should be between max and min")
  {
    int min = 0;
    int max = 10;

    for(size_t i = 0; i < max; i++)
    {
      int randomInt = random::getInteger(min, max);

      bool boundaryCheck = randomInt >= min && randomInt <= max;

      REQUIRE(boundaryCheck);
    }
  }
}