#include <catch2/catch_test_macros.hpp>
#include <util/logger.hpp>

TEST_CASE("Replacing values correctly", "[logger]")
{
  std::string tail = "\033[0m";

  SECTION("Replaces 0 and 1 from boolean with false and true")
  {
    auto value = ngyn::logger.debug("{} {}", true, false);
    REQUIRE(value.ends_with("true false" + tail));
  }

  SECTION("Replaces values using args order")
  {
    auto value = ngyn::logger.debug("{} {} {}", 0, 1, 2);
    REQUIRE(value.ends_with("0 1 2" + tail));
  }

  SECTION("Replaces values using provided indexed placeholders in order")
  {
    auto value = ngyn::logger.debug("{0} {1} {2}", 2, 3, 4);
    REQUIRE(value.ends_with("2 3 4" + tail));
  }

  SECTION("Replaces values using provided indexed placeholders out of order")
  {
    auto value = ngyn::logger.debug("{2} {0} {1}", 0, 1, 2);
    REQUIRE(value.ends_with("2 0 1" + tail));
  }

  SECTION("Replaces values using args in order and provided indexed placeholders in order")
  {
    auto value = ngyn::logger.debug("{} {0} {} {1} {} {2}", 0, 1, 2);
    REQUIRE(value.ends_with("0 0 1 1 2 2" + tail));
  }

  SECTION("Replaces values using args in order and provided indexed placeholders out of order")
  {
    auto value = ngyn::logger.debug("{} {2} {} {0} {} {1}", 0, 1, 2);
    REQUIRE(value.ends_with("0 2 1 0 2 1" + tail));
  }
}

TEST_CASE("Output color should match", "[logger]")
{
  SECTION("Debug output should have teal color")
  {
    std::string color = "0;255;255";
    auto value = ngyn::logger.debug("Test");

    REQUIRE(value.find(color) != std::string::npos);
  }

  SECTION("Warn output should have yellow color")
  {
    std::string color = "255;255;0";
    auto value = ngyn::logger.warn("Test");

    REQUIRE(value.find(color) != std::string::npos);
  }

  SECTION("Error output should have red color")
  {
    std::string color = "255;0;0";
    auto value = ngyn::logger.error("Test");

    REQUIRE(value.find(color) != std::string::npos);
  }
}