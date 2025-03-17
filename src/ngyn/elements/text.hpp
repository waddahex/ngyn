#pragma once

#include "../../ngynpch.hpp"

#include "../resources/font.hpp"
#include "../util/color.hpp"
#include "../components/camera.hpp"
#include "../renderers/quad_renderer.hpp"
#include "elements_enums.hpp"
#include "sprite.hpp"

namespace ngyn
{
  class Text
  {
    public:
    struct CreateInfo
    {
      std::weak_ptr<Font> font;
      std::weak_ptr<Camera> camera;
      std::weak_ptr<QuadRenderer> renderer;
      glm::vec2 position = glm::vec2(0.0f);
      float rotation = 0.0f;
      std::string value;
      Color color = Color(255);
      Frame::Visibility visibility = Frame::Visibility::Visible;
      Transform::LayerMode layerMode = Transform::LayerMode::Simple;
      int layer = 0;
      Alignment alignment;
    };

    public:
    Text(void) = default;
    Text(CreateInfo createInfo);

    void setValue(const std::string &value);
    void setVisibility(const Frame::Visibility &visibility);

    std::weak_ptr<Font> font();
    std::weak_ptr<Camera> camera();
    std::weak_ptr<QuadRenderer> renderer();
    const glm::vec2 &position();
    const glm::vec2 &size();
    const float &rotation();
    const std::string &value();
    const Color &color();
    const Transform::LayerMode &layerMode();
    const int &layer();
    const Alignment &alignment();

    void instantiate();
    void update();

    private:
    std::weak_ptr<Font> _font;
    std::weak_ptr<Camera> _camera;
    std::weak_ptr<QuadRenderer> _renderer;
    glm::vec2 _position;
    glm::vec2 _size;
    float _rotation;
    std::string _value;
    Color _color;
    Frame::Visibility _visibility;
    Transform::LayerMode _layerMode;
    int _layer;
    Alignment _alignment;
    bool _changed;

    std::vector<int> _indexes;
  };
};