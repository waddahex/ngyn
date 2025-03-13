#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <ngyn/ngyn.hpp>

using namespace ngyn;

TEST_CASE("stdout content")
{
  SUBCASE("stdout should contain the debug logger output")
  {
    std::stringstream buffer;
    std::streambuf *old = std::cout.rdbuf(buffer.rdbuf());

    auto value = ngLogger.debug(std::source_location::current(), "Hello");

    std::cout.rdbuf(old);
    std::string capturedOutput = buffer.str();

    CHECK(value + "\n" == capturedOutput);
  }

  SUBCASE("stdout should contain the warning logger output")
  {
    std::stringstream buffer;
    std::streambuf *old = std::cout.rdbuf(buffer.rdbuf());

    auto value = ngLogger.warning(std::source_location::current(), "Hello");

    std::cout.rdbuf(old);
    std::string capturedOutput = buffer.str();

    CHECK(value + "\n" == capturedOutput);
  }

  SUBCASE("stdout should contain the error logger output")
  {
    std::stringstream buffer;
    std::streambuf *old = std::cout.rdbuf(buffer.rdbuf());

    auto value = ngLogger.error(std::source_location::current(), "Hello");

    std::cout.rdbuf(old);
    std::string capturedOutput = buffer.str();

    CHECK(value + "\n" == capturedOutput);
  }
}

TEST_CASE("Save log to file")
{
  ngLogger.setDirectory("logs");
  std::filesystem::remove_all("logs");

  time_t timestamp = time(&timestamp);
  struct tm datetime;
  localtime_s(&datetime, &timestamp);

  auto day = datetime.tm_mday > 10 ? std::to_string(datetime.tm_mday) : "0" + std::to_string(datetime.tm_mday);
  auto month = datetime.tm_mon + 1 > 10 ? std::to_string(datetime.tm_mon + 1) : "0" + std::to_string(datetime.tm_mon + 1);
  auto year = std::to_string(datetime.tm_year + 1900);

  std::filesystem::path filename = std::format("{}-{}-{}", year, month, day);

  SUBCASE("Should not save to file if mode equal LoggerMode::Console")
  {
    ngLogger.setMode(LoggerMode::Console);
    ngLogger.error(std::source_location::current(), "Save test");

    CHECK(!std::filesystem::exists("logs" / filename));
  }

  SUBCASE("Should save to file if mode equal LoggerMode::Quiet")
  {
    ngLogger.setMode(LoggerMode::Quiet);
    ngLogger.error(std::source_location::current(), "LoggerMode::Quiet test");

    std::string logData = files::read(std::filesystem::path("logs" / filename));

    CHECK(logData.ends_with("LoggerMode::Quiet test\n"));
  }

  SUBCASE("Should save to file and print to console if mode equal LoggerMode::All")
  {
    ngLogger.setMode(LoggerMode::All);

    std::stringstream buffer;
    std::streambuf *old = std::cout.rdbuf(buffer.rdbuf());

    auto value = ngLogger.error(std::source_location::current(), "LoggerMode::All test");

    std::cout.rdbuf(old);
    std::string capturedOutput = buffer.str();

    std::string logData = files::read(std::filesystem::path("logs" / filename));

    CHECK(logData.ends_with("LoggerMode::All test\n"));
    CHECK(value + "\n" == capturedOutput);
  }

  SUBCASE("Should not save or log debug logs if level is lower than LoggerLevel::Debug")
  {
    ngLogger.setLevel(LoggerLevel::Warning);

    std::stringstream buffer;
    std::streambuf *old = std::cout.rdbuf(buffer.rdbuf());

    auto value = ngLogger.debug(std::source_location::current(), "LoggerLevel::Warning test");

    std::cout.rdbuf(old);
    std::string capturedOutput = buffer.str();

    std::string logData = files::read(std::filesystem::path("logs" / filename));

    CHECK(!logData.ends_with("LoggerLevel::Warning test\n"));
    CHECK(value + "\n" != capturedOutput);
  }

  SUBCASE("Should not save or log warning logs if level is lower than LoggerLevel::Warning")
  {
    ngLogger.setLevel(LoggerLevel::Error);

    std::stringstream buffer;
    std::streambuf *old = std::cout.rdbuf(buffer.rdbuf());

    auto value = ngLogger.warning(std::source_location::current(), "LoggerLevel::Error test");

    std::cout.rdbuf(old);
    std::string capturedOutput = buffer.str();

    std::string logData = files::read(std::filesystem::path("logs" / filename));

    CHECK(!logData.ends_with("LoggerLevel::Error test\n"));
    CHECK(value + "\n" != capturedOutput);
  }

  SUBCASE("Should not save or log anything logs if level is lower than LoggerLevel::Error")
  {
    ngLogger.setLevel(LoggerLevel::Disabled);

    std::stringstream buffer;
    std::streambuf *old = std::cout.rdbuf(buffer.rdbuf());

    auto value = ngLogger.error(std::source_location::current(), "LoggerLevel::Disabled test");

    std::cout.rdbuf(old);
    std::string capturedOutput = buffer.str();

    std::string logData = files::read(std::filesystem::path("logs" / filename));

    CHECK(!logData.ends_with("LoggerLevel::Disabled test\n"));
    CHECK(value + "\n" != capturedOutput);
  }
}

TEST_CASE("Replacing values correctly")
{
  std::string tail = "\033[0m";
  ngLogger.setMode(LoggerMode::Quiet);
  ngLogger.setLevel(LoggerLevel::Debug);

  SUBCASE("Replaces 0 from boolean with false")
  {
    auto value = ngLogger.debug(std::source_location::current(), false);
    CHECK(value.ends_with("false" + tail));
  }

  SUBCASE("Replaces 1 from boolean with true")
  {
    auto value = ngLogger.debug(std::source_location::current(), true);
    CHECK(value.ends_with("true" + tail));
  }

  SUBCASE("Prints a single value correctly")
  {
    auto value = ngLogger.debug(std::source_location::current(), 1);
    CHECK(value.ends_with("1" + tail));
  }

  SUBCASE("Replaces values using args order")
  {
    auto value = ngLogger.debug(std::source_location::current(), "{} {} {}", 0, 1, 2);
    CHECK(value.ends_with("0 1 2" + tail));
  }

  SUBCASE("Replaces values using provided indexed placeholders in order")
  {
    auto value = ngLogger.debug(std::source_location::current(), "{0} {1} {2}", 2, 3, 4);
    CHECK(value.ends_with("2 3 4" + tail));
  }

  SUBCASE("Replaces values using provided indexed placeholders out of order")
  {
    auto value = ngLogger.debug(std::source_location::current(), "{2} {0} {1}", 0, 1, 2);
    CHECK(value.ends_with("2 0 1" + tail));
  }

  SUBCASE("Replaces values using args in order and provided indexed placeholders in order")
  {
    auto value = ngLogger.debug(std::source_location::current(), "{} {0} {} {1} {} {2}", 0, 1, 2);
    CHECK(value.ends_with("0 0 1 1 2 2" + tail));
  }

  SUBCASE("Replaces values using args in order and provided indexed placeholders out of order")
  {
    auto value = ngLogger.debug(std::source_location::current(), "{} {2} {} {0} {} {1}", 0, 1, 2);
    CHECK(value.ends_with("0 2 1 0 2 1" + tail));
  }
}

TEST_CASE("Output color should match")
{
  ngLogger.setMode(LoggerMode::Quiet);
  ngLogger.setLevel(LoggerLevel::Debug);

  SUBCASE("Debug output should have teal color")
  {
    std::string color = "0;255;255";
    auto value = ngLogger.debug(std::source_location::current(), "Test");

    CHECK(value.find(color) != std::string::npos);
  }

  SUBCASE("Warn output should have yellow color")
  {
    std::string color = "255;255;0";
    auto value = ngLogger.warning(std::source_location::current(), "Test");

    CHECK(value.find(color) != std::string::npos);
  }

  SUBCASE("Error output should have red color")
  {
    std::string color = "255;0;0";
    auto value = ngLogger.error(std::source_location::current(), "Test");

    CHECK(value.find(color) != std::string::npos);
  }
}