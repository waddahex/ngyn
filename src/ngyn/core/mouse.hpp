#pragma once

#include "../../ngynpch.hpp"

namespace ngyn
{
  class Mouse
  {
    public:
    Mouse(void) = default;

    static const glm::vec2 &client();
    static void setClient(const glm::vec2 &position);
    static const glm::vec2 &world();
    static void setWorld(const glm::vec2 &position);

    private:
    static glm::vec2 _client;
    static glm::vec2 _world;
  };
};