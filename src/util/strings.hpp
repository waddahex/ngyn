#pragma once

#include <string>

namespace ngyn::strings
{
  std::string replace(const std::string &string, const std::string &pattern, const std::string &replacement);
  std::string replaceAll(const std::string &string, const std::string &pattern, const std::string &replacement);
};