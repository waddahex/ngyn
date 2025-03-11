#pragma once

#include "../../ngynpch.hpp"

#include "strings.hpp"

namespace ngyn::random
{
  int getInteger(int min, int max);
  float getFloat(float min, float max);
  std::string uuid();
};