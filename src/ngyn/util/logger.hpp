#pragma once

#include <iostream>
#include <sstream>
#include <format>
#include <ctime>
#include <unordered_map>
#include <filesystem>

#include "strings.hpp"
#include "files.hpp"

namespace ngyn
{
  enum LoggerMode
  {
    All     = 0,  // Saves to file and prints to the console
    Quiet   = 1,  // Only saves to file
    Console = 2,  // Only prints to the console
  };

  enum LoggerLevel
  {
    Disabled  = 0,
    Error     = 1,
    Warning   = 2,
    Debug     = 3,
  };

  class Logger
  {
    public:
    Logger(void) = default;

    template<typename... Args>
    std::string error(Args&&... args)
    {
      if(LoggerLevel::Error > this->level)
      {
        return std::string();
      }

      log(std::forward<Args>(args)...);
      print(LoggerLevel::Error, 255, 0, 0);
      return buffer;
    }

    template<typename... Args>
    std::string warning(Args&&... args)
    {
      if(LoggerLevel::Warning > this->level)
      {
        return std::string();
      }

      log(std::forward<Args>(args)...);
      print(LoggerLevel::Warning, 255, 255, 0);

      return buffer;
    }

    template<typename... Args>
    std::string debug(Args&&... args)
    {
      if(LoggerLevel::Debug > this->level)
      {
        return std::string();
      }

      log(std::forward<Args>(args)...);
      print(LoggerLevel::Debug, 0, 255, 255);

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

    void setDirectory(const std::filesystem::path &path)
    {
      if(!std::filesystem::exists(path))
      {
        return;
      }

      this->directory = path;
    }

    void setLevel(LoggerLevel level)
    {
      this->level = level;
    }

    private:
    std::string buffer; // used to store the final string
    std::string format = "yyyy-MM-dd HH:mm:ss $T";
    std::filesystem::path directory = "logs";
    LoggerMode mode = LoggerMode::All;
    LoggerLevel level = LoggerLevel::Debug;

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

    void print(LoggerLevel currentLevel, int r, int g, int b)
    {
      std::string type;
      switch(currentLevel)
      {
        case LoggerLevel::Debug:
          type = "DEBUG";
          break;
        case LoggerLevel::Warning:
          type = "WARNING";
          break;
        case LoggerLevel::Error:
          type = "ERROR";
          break;
      }

      auto formatted = getReplacedFormatString(this->format, type);

      std::ostringstream sstream;

      sstream << std::format("\033[1m[\033[38;2;{};{};{}m{}\033[0;1m]: ", r, g, b, formatted);
      sstream << buffer;
      sstream << "\033[0m";

      if(mode != LoggerMode::Quiet)
      {
        std::cout << sstream.str() << std::endl;
      }

      if(mode != LoggerMode::Console)
      {
        time_t timestamp = time(&timestamp);
        struct tm datetime;
        localtime_s(&datetime, &timestamp);
  
        std::string formattedName = getReplacedFormatString("yyyy-MM-dd HH:mm:ss $T", type);
        std::filesystem::path filePath = this->directory / formattedName.substr(0, 10);

        ngyn::files::write(filePath, std::format("[{}]: {}\n", formattedName, buffer), {
          .append = true,
          .recursive = true
        });
      }

      buffer = sstream.str();
    }

    std::string getReplacedFormatString(const std::string &format, const std::string &type)
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
#define LOGGER_WARNING(...) ngyn::logger.warn(__VA_ARGS__);
#define ASSERT(condition, ...) \
  if(!(condition)) /* Using "(condition)" for logical AND operator */ \
  { \
    ngyn::logger.error(##__VA_ARGS__); \
    DEBUG_BREAK; \
  }