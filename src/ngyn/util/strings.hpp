#pragma once

#include <string>
#include <cstring>

namespace ngyn::strings
{
  std::string replace(const std::string &string, const std::string &pattern, const std::string &replacement);
  std::string replaceAll(const std::string &string, const std::string &pattern, const std::string &replacement);
  std::string uppercase(const std::string &string);
  std::string lowercase(const std::string &string);
};