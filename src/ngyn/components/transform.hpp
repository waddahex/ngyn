#pragma once

#include "../../ngynpch.hpp"

/*
  TODO: Add origin for scale, rotation?
 */

namespace ngyn
{
  class Transform
  {
    public:
    enum LayerMode
    {
      Simple,
      Dynamic
    };

    struct CreateInfo
    {
      glm::vec2 position = glm::vec2(0.0f);
      glm::vec2 size = glm::vec2(0.0f);
      glm::vec2 scale = glm::vec2(1.0f);
      float rotation = 0.0f;
      LayerMode layerMode = LayerMode::Simple;
      int layer = 0;
    };

    public:
    Transform(void) = default;
    Transform(CreateInfo createInfo);
    
    // Getters
    const glm::vec2 &position();
    const glm::vec2 &size();
    const glm::vec2 &scale();
    const float &rotation();
    const LayerMode &layerMode();
    const int &layer();
    const glm::mat4 &model();
    const float &zIndex();

    // Setters
    void setPosition(const glm::vec2 &position);
    void setSize(const glm::vec2 &size);
    void setScale(const glm::vec2 &scale);
    void setRotation(const float &rotation);
    void setLayerMode(const LayerMode &layerMode);
    void setLayer(const int &layer);

    // Incremental methods
    void moveBy(const glm::vec2 &velocity);
    void resizeBy(const glm::vec2 &size);
    void scaleBy(const glm::vec2 &scale);
    void rotateBy(const float &deg);

    private:
    // CreateInfo properties
    glm::vec2 _position;
    glm::vec2 _size;
    glm::vec2 _scale;
    float _rotation;
    LayerMode _layerMode;
    int _layer;

    // Other properties
    glm::mat4 _model;
    float _zIndex;

    void updateModel();
  };
};