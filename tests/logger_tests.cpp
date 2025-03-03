#include <catch2/catch_test_macros.hpp>
#include <ngyn/util/logger.hpp>

using namespace ngyn;

TEST_CASE("stdout content", "[logger]")
{
  SECTION("stdout should contain the debug logger output")
  {
    std::stringstream buffer;
    std::streambuf *old = std::cout.rdbuf(buffer.rdbuf());

    auto value = ngyn::logger.debug("Hello");

    std::cout.rdbuf(old);
    std::string capturedOutput = buffer.str();

    REQUIRE(value + "\n" == capturedOutput);
  }

  SECTION("stdout should contain the warn logger output")
  {
    std::stringstream buffer;
    std::streambuf *old = std::cout.rdbuf(buffer.rdbuf());

    auto value = ngyn::logger.warn("Hello");

    std::cout.rdbuf(old);
    std::string capturedOutput = buffer.str();

    REQUIRE(value + "\n" == capturedOutput);
  }

  SECTION("stdout should contain the error logger output")
  {
    std::stringstream buffer;
    std::streambuf *old = std::cout.rdbuf(buffer.rdbuf());

    auto value = ngyn::logger.error("Hello");

    std::cout.rdbuf(old);
    std::string capturedOutput = buffer.str();

    REQUIRE(value + "\n" == capturedOutput);
  }
}

TEST_CASE("Replacing values correctly", "[logger]")
{
  std::string tail = "\033[0m";
  ngyn::logger.setMode(LoggerMode::Quiet);

  SECTION("Replaces 0 from boolean with false")
  {
    auto value = ngyn::logger.debug(false);
    REQUIRE(value.ends_with("false" + tail));
  }

  SECTION("Replaces 1 from boolean with true")
  {
    auto value = ngyn::logger.debug(true);
    REQUIRE(value.ends_with("true" + tail));
  }

  SECTION("Prints a single value correctly")
  {
    auto value = ngyn::logger.debug(1);
    REQUIRE(value.ends_with("1" + tail));
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
  ngyn::logger.setMode(LoggerMode::Quiet);

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