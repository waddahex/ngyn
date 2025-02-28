#pragma once

#include <iostream>
#include <sstream>
#include <format>
#include <ctime>
#include <unordered_map>

namespace ngyn
{
  class Logger
  {
    public:
    Logger(void) = default;

    template<typename... Args>
    std::string error(Args&&... args)
    {
      log(std::forward<Args>(args)...);
      return print("ERROR", 255, 0, 0);
    }

    template<typename... Args>
    std::string warn(Args&&... args)
    {
      log(std::forward<Args>(args)...);
      return print("WARNING", 255, 255, 0);
    }

    template<typename... Args>
    std::string debug(Args&&... args)
    {
      log(std::forward<Args>(args)...);
      return print("DEBUG", 0, 255, 255);
    }

    void setFormat(const std::string &format)
    {
      this->format = format;
    }

    private:
    std::string buffer; // used to store the final string
    std::string format = "dd/MM/yyyy HH:mm:ss $T";

    // This version of log accepts one argument for simple logging
    template<typename T>
    void log(const T &fmt)
    {
      replace(0, fmt);
    }

    template<typename T, typename... Args>
    void log(const T &fmt, Args... args)
    {
      buffer = fmt;
      size_t index = 0;

      // Run replace using fold expression recursivelly
      (..., replace(index++, args));
    }

    template<typename T>
    void replace(size_t index, T value)
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

      // Gets the first {} on the fmt string and replace it for the current value
      size_t pos = buffer.find("{}");
      if(pos != std::string::npos)
      {
        buffer.replace(pos, 2, sstream.str());
      }

      // Gets the position of {n} and replace it for the current value while it exists
      pos = buffer.find("{" + std::to_string(index) + "}");
      while(pos != std::string::npos)
      {
        buffer.replace(pos, 3, sstream.str());
        pos = buffer.find("{" + std::to_string(index) + "}");
      }
    }

    std::string print(const std::string &type, int r, int g, int b)
    {
      auto formatted = getReplacedFormatString(type);

      std::ostringstream sstream;

      sstream << std::format("\033[1m[\033[38;2;{};{};{}m{}\033[0;1m]: ", r, g, b, formatted);
      sstream << buffer;
      sstream << "\033[0m";

      return sstream.str();
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
        size_t pos = formatted.find(key);
        while(pos != std::string::npos)
        {
          formatted.replace(pos, key.size(), value);
          pos = formatted.find(key);
        }
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

#define LOGGER_DEBUG(...) std::cout << ngyn::logger.debug(__VA_ARGS__) << std::endl;
#define LOGGER_ERROR(...) std::cout << ngyn::logger.error(__VA_ARGS__) << std::endl;
#define LOGGER_WARN(...) std::cout << ngyn::logger.warn(__VA_ARGS__) << std::endl;
#define ASSERT(condition, ...) \
  if(!condition) \
  { \
    std::cout << ngyn::logger.error(##__VA_ARGS__) << std::endl; \
    DEBUG_BREAK; \
  }