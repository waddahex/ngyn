#pragma once

#include <iostream>

namespace ddengine
{
  class Logger
  {
    public:
    Logger(void) = default;

    void log(const std::string &fmt);
  };

  inline Logger logger;
};