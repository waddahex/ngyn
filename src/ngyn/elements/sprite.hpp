#pragma once

#include "../../ngynpch.hpp"

#include "../components/frame.hpp"
#include "../components/transform.hpp"
#include "../components/camera.hpp"
#include "../renderers/renderer.hpp"
#include "../renderers/quad_renderer.hpp"

/*
  TODO: Move the increment methods from Transform to Sprite
*/

namespace ngyn
{
  class Sprite
  {
    public:
    struct CreateInfo
    {
      Frame::CreateInfo frame = Frame::CreateInfo{};
      Transform::CreateInfo transform = Transform::CreateInfo{};
      std::shared_ptr<QuadRenderer> renderer;
      std::shared_ptr<Camera> camera;
    };

    public:
    Sprite(void) = default;
    Sprite(CreateInfo createInfo);

    Frame frame;
    Transform transform;

    void instantiate();
    void update();

    private:
    int _instanceIndex;
    std::shared_ptr<QuadRenderer> _renderer;
    std::shared_ptr<Camera> _camera;
  };
};