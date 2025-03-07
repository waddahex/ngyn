#include <catch2/catch_test_macros.hpp>
#include <ngyn/util/logger.hpp>
#include <ngyn/util/files.hpp>

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

  SECTION("stdout should contain the warning logger output")
  {
    std::stringstream buffer;
    std::streambuf *old = std::cout.rdbuf(buffer.rdbuf());

    auto value = ngyn::logger.warning("Hello");

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

TEST_CASE("Save log to file", "[logger]")
{
  ngyn::logger.setDirectory("logs");
  std::filesystem::remove_all("logs");

  time_t timestamp = time(&timestamp);
  struct tm datetime;
  localtime_s(&datetime, &timestamp);

  auto day = datetime.tm_mday > 10 ? std::to_string(datetime.tm_mday) : "0" + std::to_string(datetime.tm_mday);
  auto month = datetime.tm_mon + 1 > 10 ? std::to_string(datetime.tm_mon + 1) : "0" + std::to_string(datetime.tm_mon + 1);
  auto year = std::to_string(datetime.tm_year + 1900);

  std::filesystem::path filename = std::format("{}-{}-{}", year, month, day);

  SECTION("Should not save to file if mode equal LoggerMode::Console")
  {
    ngyn::logger.setMode(LoggerMode::Console);
    ngyn::logger.error("Save test");

    REQUIRE(!std::filesystem::exists("logs" / filename));
  }

  SECTION("Should save to file if mode equal LoggerMode::Quiet")
  {
    ngyn::logger.setMode(LoggerMode::Quiet);
    ngyn::logger.error("LoggerMode::Quiet test");

    std::string logData = files::read(std::filesystem::path("logs" / filename));

    REQUIRE(logData.ends_with("LoggerMode::Quiet test\n"));
  }

  SECTION("Should save to file and print to console if mode equal LoggerMode::All")
  {
    ngyn::logger.setMode(LoggerMode::All);

    std::stringstream buffer;
    std::streambuf *old = std::cout.rdbuf(buffer.rdbuf());

    auto value = ngyn::logger.error("LoggerMode::All test");

    std::cout.rdbuf(old);
    std::string capturedOutput = buffer.str();

    std::string logData = files::read(std::filesystem::path("logs" / filename));

    REQUIRE(logData.ends_with("LoggerMode::All test\n"));
    REQUIRE(value + "\n" == capturedOutput);
  }

  SECTION("Should not save or log debug logs if level is lower than LoggerLevel::Debug")
  {
    ngyn::logger.setLevel(LoggerLevel::Warning);

    std::stringstream buffer;
    std::streambuf *old = std::cout.rdbuf(buffer.rdbuf());

    auto value = ngyn::logger.debug("LoggerLevel::Warning test");

    std::cout.rdbuf(old);
    std::string capturedOutput = buffer.str();

    std::string logData = files::read(std::filesystem::path("logs" / filename));

    REQUIRE(!logData.ends_with("LoggerLevel::Warning test\n"));
    REQUIRE(value + "\n" != capturedOutput);
  }

  SECTION("Should not save or log warning logs if level is lower than LoggerLevel::Warning")
  {
    ngyn::logger.setLevel(LoggerLevel::Error);

    std::stringstream buffer;
    std::streambuf *old = std::cout.rdbuf(buffer.rdbuf());

    auto value = ngyn::logger.warning("LoggerLevel::Error test");

    std::cout.rdbuf(old);
    std::string capturedOutput = buffer.str();

    std::string logData = files::read(std::filesystem::path("logs" / filename));

    REQUIRE(!logData.ends_with("LoggerLevel::Error test\n"));
    REQUIRE(value + "\n" != capturedOutput);
  }

  SECTION("Should not save or log anything logs if level is lower than LoggerLevel::Error")
  {
    ngyn::logger.setLevel(LoggerLevel::Disabled);

    std::stringstream buffer;
    std::streambuf *old = std::cout.rdbuf(buffer.rdbuf());

    auto value = ngyn::logger.error("LoggerLevel::Disabled test");

    std::cout.rdbuf(old);
    std::string capturedOutput = buffer.str();

    std::string logData = files::read(std::filesystem::path("logs" / filename));

    REQUIRE(!logData.ends_with("LoggerLevel::Disabled test\n"));
    REQUIRE(value + "\n" != capturedOutput);
  }
}

TEST_CASE("Replacing values correctly", "[logger]")
{
  std::string tail = "\033[0m";
  ngyn::logger.setMode(LoggerMode::Quiet);
  ngyn::logger.setLevel(LoggerLevel::Debug);

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
  ngyn::logger.setLevel(LoggerLevel::Debug);

  SECTION("Debug output should have teal color")
  {
    std::string color = "0;255;255";
    auto value = ngyn::logger.debug("Test");

    REQUIRE(value.find(color) != std::string::npos);
  }

  SECTION("Warn output should have yellow color")
  {
    std::string color = "255;255;0";
    auto value = ngyn::logger.warning("Test");

    REQUIRE(value.find(color) != std::string::npos);
  }

  SECTION("Error output should have red color")
  {
    std::string color = "255;0;0";
    auto value = ngyn::logger.error("Test");

    REQUIRE(value.find(color) != std::string::npos);
  }
}