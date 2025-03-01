#include "strings.hpp"

std::string ngyn::strings::replace(const std::string &string, const std::string &pattern, const std::string &replacement)
{
  std::string replaced = string;

  size_t pos = replaced.find(pattern);
  if(pos != std::string::npos)
  {
    replaced.replace(pos, pattern.size(), replacement);
  }

  return std::move(replaced);
}