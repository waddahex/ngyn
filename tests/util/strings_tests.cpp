#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <ngyn/ngyn.hpp>

using namespace ngyn;

TEST_CASE("Replace values correctly")
{
  SUBCASE("Replace a single")
  {
    std::string pattern = "test";
    std::string replacement = "world";
    std::string replaced = strings::replace("replace test", pattern, replacement);
    CHECK(replaced == "replace " + replacement);
  }

  SUBCASE("Replace multiple worlds")
  {
    std::string pattern = "replace test";
    std::string replacement = "hello world";
    std::string replaced = strings::replace("replace test", pattern, replacement);
    CHECK(replaced == replacement);
  }

  SUBCASE("Replace pattern for empty string")
  {
    std::string pattern = "replace test";
    std::string replacement = "";
    std::string replaced = strings::replace("replace test", pattern, replacement);
    CHECK(replaced == replacement);
  }

  SUBCASE("Replace all values single word pattern")
  {
    std::string pattern = "1";
    std::string replacement = "2";
    std::string replaced = strings::replaceAll("a1 b1 c1", pattern, replacement);
    CHECK(replaced == "a2 b2 c2");
  }

  SUBCASE("Replace all values multiple words pattern")
  {
    std::string pattern = "replace test";
    std::string replacement = "2";
    std::string replaced = strings::replaceAll(
      "areplace test breplace test creplace test",
      pattern,
      replacement
    );
    CHECK(replaced == "a2 b2 c2");
  }
}

TEST_CASE("Transformations")
{
  SUBCASE("Returns the input converted into uppercase")
  {
    std::string uppercased = strings::uppercase("uppercase123$");
    CHECK(uppercased == "UPPERCASE123$");
  }

  SUBCASE("Returns the input converted into lowercase")
  {
    std::string lowercased = strings::lowercase("LOWERCASE123@");
    CHECK(lowercased == "lowercase123@");
  }
}