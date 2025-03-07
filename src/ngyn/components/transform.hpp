#pragma once

#include "../../ngynpch.hpp"

namespace ngyn
{
  struct TransformCreateInfo
  {
    glm::vec3 position;
    glm::vec3 size;
    glm::vec3 scale;
    glm::vec3 rotation;
    int layer;
  };

  class Transform
  {
    public:
    Transform(void) = default;
    Transform(TransformCreateInfo createInfo);

    glm::vec3 position;
    glm::vec3 size;
    glm::vec3 scale;
    glm::vec3 rotation;
    int layer;
    glm::mat4 model;
    float zIndex;

    private:
    void updateModel();
  };
};