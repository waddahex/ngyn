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

    const int &instanceIndex();
    std::shared_ptr<QuadRenderer> renderer();
    std::shared_ptr<Camera> camera();

    void setRenderer(std::shared_ptr<QuadRenderer> renderer);
    void setCamera(std::shared_ptr<Camera> camera);

    // Incremental methods
    void moveBy(const glm::vec2 &velocity);
    void resizeBy(const glm::vec2 &size);
    void scaleBy(const glm::vec2 &scale);
    void rotateBy(const float &deg);

    void instantiate();
    void update();

    private:
    int _instanceIndex;
    std::shared_ptr<QuadRenderer> _renderer;
    std::shared_ptr<Camera> _camera;

    QuadInstanceData getData();
  };
};