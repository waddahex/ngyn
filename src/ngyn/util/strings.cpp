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

std::string ngyn::strings::replaceAll(const std::string &string, const std::string &pattern, const std::string &replacement)
{
  std::string replaced = replace(string, pattern, replacement);

  if(replaced.find(pattern) != std::string::npos)
  {
    replaced = replaceAll(replaced, pattern, replacement);
  }

  return replaced;
}

std::string ngyn::strings::uppercase(const std::string &string)
{
  std::string uppercased;

  for(auto c : string)
  {
    uppercased += toupper(c);
  }

  return std::move(uppercased);
}

std::string ngyn::strings::lowercase(const std::string &string)
{
  std::string lowercased;

  for(auto c : string)
  {
    lowercased += tolower(c);
  }

  return std::move(lowercased);
}
