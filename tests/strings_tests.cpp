#include <catch2/catch_test_macros.hpp>
#include <util/strings.hpp>

TEST_CASE("Replace values correctly", "[strings]")
{
  SECTION("Replace a single")
  {
    std::string replacement = "world";
    std::string replaced = ngyn::strings::replace("replace test", "test", replacement);
    REQUIRE(replaced == "replace " + replacement);
  }

  SECTION("Replace multiple worlds")
  {
    std::string replacement = "hello world";
    std::string replaced = ngyn::strings::replace("replace test", "replace test", replacement);
    REQUIRE(replaced == replacement);
  }

  SECTION("Replace pattern for empty string")
  {
    std::string replacement = "";
    std::string replaced = ngyn::strings::replace("replace test", "replace test", replacement);
    REQUIRE(replaced == replacement);
  }
}