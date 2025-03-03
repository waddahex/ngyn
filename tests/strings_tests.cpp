#include <catch2/catch_test_macros.hpp>
#include <ngyn/util/strings.hpp>

using namespace ngyn;

TEST_CASE("Replace values correctly", "[strings]")
{
  SECTION("Replace a single")
  {
    std::string pattern = "test";
    std::string replacement = "world";
    std::string replaced = strings::replace("replace test", pattern, replacement);
    REQUIRE(replaced == "replace " + replacement);
  }

  SECTION("Replace multiple worlds")
  {
    std::string pattern = "replace test";
    std::string replacement = "hello world";
    std::string replaced = strings::replace("replace test", pattern, replacement);
    REQUIRE(replaced == replacement);
  }

  SECTION("Replace pattern for empty string")
  {
    std::string pattern = "replace test";
    std::string replacement = "";
    std::string replaced = strings::replace("replace test", pattern, replacement);
    REQUIRE(replaced == replacement);
  }

  SECTION("Replace all values single word pattern")
  {
    std::string pattern = "1";
    std::string replacement = "2";
    std::string replaced = strings::replaceAll("a1 b1 c1", pattern, replacement);
    REQUIRE(replaced == "a2 b2 c2");
  }

  SECTION("Replace all values multiple words pattern")
  {
    std::string pattern = "replace test";
    std::string replacement = "2";
    std::string replaced = strings::replaceAll(
      "areplace test breplace test creplace test",
      pattern,
      replacement
    );
    REQUIRE(replaced == "a2 b2 c2");
  }
}

TEST_CASE("Transformations", "[strings]")
{
  SECTION("Returns the input converted into uppercase")
  {
    std::string uppercased = strings::uppercase("uppercase123$");
    REQUIRE(uppercased == "UPPERCASE123$");
  }

  SECTION("Returns the input converted into lowercase")
  {
    std::string lowercased = strings::lowercase("LOWERCASE123@");
    REQUIRE(lowercased == "lowercase123@");
  }
}