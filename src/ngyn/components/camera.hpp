#pragma once

#include "../../ngynpch.hpp"

namespace ngyn
{
  class Camera
  {
    public:
    struct CreateInfo
    {
      glm::vec2 position = glm::vec2(0.0f);
      glm::vec2 resolution = glm::vec2(0.0f);
    };

    public:
    Camera(void) = default;
    Camera(CreateInfo createInfo);

    const glm::vec2 &position();
    const glm::vec2 &resolution();
    const glm::mat4 &projection();
    const glm::mat4 &view();

    void setPosition(const glm::vec2 &position);
    void setResolution(const glm::vec2 &resolution);

    glm::vec2 _position;
    glm::vec2 _resolution;
    glm::mat4 _projection;
    glm::mat4 _view;

    void update();
  };
};