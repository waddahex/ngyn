#pragma once

#include <iostream>
#include <sstream>
#include <format>
#include <ctime>
#include <unordered_map>

#include "strings.hpp"

namespace ngyn
{
  enum LoggerMode
  {
    All = 0,
    Quiet = 1,
    Console = 2,
    File = 3
  };

  class Logger
  {
    public:
    Logger(void) = default;

    template<typename... Args>
    std::string error(Args&&... args)
    {
      log(std::forward<Args>(args)...);
      print("ERROR", 255, 0, 0);
      return buffer;
    }

    template<typename... Args>
    std::string warn(Args&&... args)
    {
      log(std::forward<Args>(args)...);
      print("WARNING", 255, 255, 0);

      return buffer;
    }

    template<typename... Args>
    std::string debug(Args&&... args)
    {
      log(std::forward<Args>(args)...);
      print("DEBUG", 0, 255, 255);

      return buffer;
    }

    void setFormat(const std::string &format)
    {
      this->format = format;
    }

    void setMode(LoggerMode mode)
    {
      this->mode = mode;
    }

    private:
    std::string buffer; // used to store the final string
    std::string format = "dd/MM/yyyy HH:mm:ss $T";
    LoggerMode mode = LoggerMode::All;

    // This version of log accepts one argument for simple logging
    template<typename T>
    void log(const T &fmt)
    {
      buffer = convertValue(fmt);
    }

    template<typename T, typename... Args>
    void log(const T &fmt, Args... args)
    {
      buffer = fmt;
      size_t index = 0;

      // Run replace using fold expression recursivelly
      (..., replace(index++, convertValue(args)));
    }

    template<typename T>
    std::string convertValue(T value)
    {
      std::ostringstream sstream;
      if(std::is_same_v<T, bool>)
      {
        sstream << std::boolalpha << value;
      }
      else
      {
        sstream << value;
      }

      return sstream.str();
    }

    void replace(size_t index, const std::string &value)
    {
      // Gets the first {} on the fmt string and replace it for the current value
      buffer = ngyn::strings::replace(buffer, "{}", value);

      // Gets the position of {n} and replace it for the current value while it exists
      buffer = ngyn::strings::replaceAll(buffer, "{" + std::to_string(index) + "}", value);
    }

    void print(const std::string &type, int r, int g, int b)
    {
      auto formatted = getReplacedFormatString(type);

      std::ostringstream sstream;

      sstream << std::format("\033[1m[\033[38;2;{};{};{}m{}\033[0;1m]: ", r, g, b, formatted);
      sstream << buffer;
      sstream << "\033[0m";

      buffer = sstream.str();

      if(mode == LoggerMode::Quiet)
      {
        return;
      }

      std::cout << buffer << std::endl;

      // TODO: Save output to file if enabled
    }

    std::string getReplacedFormatString(const std::string &type)
    {
      time_t timestamp = time(&timestamp);
      struct tm datetime;
      localtime_s(&datetime, &timestamp);

      std::unordered_map<std::string, std::string> values = {
        { "dd", datetime.tm_mday > 10 ? std::to_string(datetime.tm_mday) : "0" + std::to_string(datetime.tm_mday) },
        { "MM", datetime.tm_mon + 1 > 10 ? std::to_string(datetime.tm_mon + 1) : "0" + std::to_string(datetime.tm_mon + 1) },
        { "yyyy", std::to_string(datetime.tm_year + 1900) },
        { "HH", datetime.tm_hour > 10 ? std::to_string(datetime.tm_hour) : "0" + std::to_string(datetime.tm_hour) },
        { "mm", datetime.tm_min > 10 ? std::to_string(datetime.tm_min) : "0" + std::to_string(datetime.tm_min) },
        { "ss", datetime.tm_sec > 10 ? std::to_string(datetime.tm_sec) : "0" + std::to_string(datetime.tm_sec) },
        { "$T", type },
      };

      std::string formatted = format;

      for(auto [key, value]: values)
      {
        formatted = ngyn::strings::replaceAll(formatted, key, value);
      }

      return std::move(formatted);
    }
  };

  inline Logger logger;
};

// TODO: Should check on other platforms in the future
#ifdef _MSC_VER
#define DEBUG_BREAK __debugbreak()
#else
#define DEBUG_BREAK 0
#endif

#define LOGGER_DEBUG(...) ngyn::logger.debug(__VA_ARGS__);
#define LOGGER_ERROR(...) ngyn::logger.error(__VA_ARGS__);
#define LOGGER_WARN(...) ngyn::logger.warn(__VA_ARGS__);
#define ASSERT(condition, ...) \
  if(!(condition)) /* Using "(condition)" for logical AND operator */ \
  { \
    ngyn::logger.error(##__VA_ARGS__); \
    DEBUG_BREAK; \
  }