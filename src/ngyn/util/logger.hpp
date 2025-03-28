#pragma once

#include "../../ngynpch.hpp"

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
    std::string error(const std::source_location location, Args&&... args)
    {
      if(LoggerLevel::Error > this->level)
      {
        return std::string();
      }

      this->location = location;

      log(std::forward<Args>(args)...);
      print(LoggerLevel::Error, 255, 0, 0);
      return buffer;
    }

    template<typename... Args>
    std::string warning(const std::source_location location, Args&&... args)
    {
      if(LoggerLevel::Warning > this->level)
      {
        return std::string();
      }

      this->location = location;

      log(std::forward<Args>(args)...);
      print(LoggerLevel::Warning, 255, 255, 0);

      return buffer;
    }

    template<typename... Args>
    std::string debug(const std::source_location location, Args&&... args)
    {
      if(LoggerLevel::Debug > this->level)
      {
        return std::string();
      }

      this->location = location;

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
    std::string format = "HH:mm:ss - $N($L:$C)";
    std::filesystem::path directory = "logs";
    LoggerMode mode = LoggerMode::All;
    LoggerLevel level = LoggerLevel::Debug;
    std::source_location location = std::source_location::current();

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
        { "dd", datetime.tm_mday < 10 ? "0" + std::to_string(datetime.tm_mday) : std::to_string(datetime.tm_mday) },
        { "MM", datetime.tm_mon + 1 < 10 ? "0" + std::to_string(datetime.tm_mon + 1) : std::to_string(datetime.tm_mon + 1) },
        { "yyyy", std::to_string(datetime.tm_year + 1900) },
        { "HH", datetime.tm_hour < 10 ? "0" + std::to_string(datetime.tm_hour) : std::to_string(datetime.tm_hour) },
        { "mm", datetime.tm_min < 10 ? "0" + std::to_string(datetime.tm_min) : std::to_string(datetime.tm_min) },
        { "ss", datetime.tm_sec < 10 ? "0" + std::to_string(datetime.tm_sec) : std::to_string(datetime.tm_sec) },
        { "$T", type },
        { "$N", location.file_name() },
        { "$L", std::to_string(location.line()) },
        { "$C", std::to_string(location.column()) },
        { "$F", location.function_name() }
      };

      std::string formatted = format;

      for(auto [key, value]: values)
      {
        formatted = ngyn::strings::replaceAll(formatted, key, value);
      }

      return std::move(formatted);
    }
  };

  inline Logger ngLogger;
};

template<typename T, glm::length_t L>
std::ostream &operator<<(std::ostream &os, const glm::vec<L, T> &vec)
{
  os << "(";

  for(size_t i = 0; i < L; ++i)
  {
    if(std::is_same_v<T, bool>) os << std::boolalpha;

    os << vec[i];

    if(i < L - 1) os << ", ";
  }

  os << ")";

  return os;
}

template<typename T, glm::length_t X, glm::length_t Y>
std::ostream &operator<<(std::ostream &os, const glm::mat<X, Y, T> &mat)
{
  for(size_t x = 0; x < X; x++)
  {
    os << "[";

    for(size_t y = 0; y < Y; y++)
    {
      os << mat[x][y];

      if(y < Y - 1) os << ", ";
    }

    os << "]";

    if(x < X - 1) os << ", ";
  }

  return os;
}

template<typename T>
std::ostream &operator<<(std::ostream &os, const std::vector<T> vector)
{
  os << "[";
  
  for(size_t i = 0; i < vector.size(); i++)
  {
    if(std::is_same_v<T, bool>) os << std::boolalpha;

    os << vector[i];

    if(i < vector.size() - 1) os << ", ";
  }

  os << "]";

  return os;
}

// TODO: Should check on other platforms in the future
#ifdef _MSC_VER
#define DEBUG_BREAK __debugbreak()
#else
#define DEBUG_BREAK 0
#endif

#define LOGGER_DEBUG(...) ngLogger.debug(std::source_location::current(), __VA_ARGS__);
#define LOGGER_ERROR(...) ngLogger.error(std::source_location::current(), __VA_ARGS__);
#define LOGGER_WARNING(...) ngLogger.warning(std::source_location::current(), __VA_ARGS__);
#define ASSERT(condition, ...) \
  if(!(condition)) /* Using "(condition)" for logical AND operator */ \
  { \
    ngLogger.error(std::source_location::current(), ##__VA_ARGS__); \
    DEBUG_BREAK; \
  }