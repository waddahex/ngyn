#pragma once

#include "../../ngynpch.hpp"
#include "../resources/texture.hpp"

namespace ngyn
{
  struct FrameCreateInfo
  {
    std::shared_ptr<Texture> texture = nullptr;
    glm::vec2 offset = glm::vec2(0.0f);
    glm::vec2 size = glm::vec2(0.0f);
    glm::bvec2 flip = glm::bvec2(false, false);
  };

  class Frame
  {
    public:
    Frame(void) = default;
    Frame(FrameCreateInfo createInfo);

    std::shared_ptr<Texture> texture;
    glm::vec2 offset;
    glm::vec2 size;
    glm::bvec2 flip;
    glm::vec4 texCoords1;
    glm::vec4 texCoords2;

    void updateTexCoords();
  };
};