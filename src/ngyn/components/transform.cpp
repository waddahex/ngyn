#include "transform.hpp"

using namespace ngyn;

Transform::Transform(CreateInfo createInfo) :
  _position(createInfo.position),
  _size(createInfo.size),
  _scale(createInfo.scale),
  _rotation(createInfo.rotation),
  _layerMode(createInfo.layerMode),
  _layer(createInfo.layer),
  _model(glm::mat4(1.0f)),
  _zIndex(0.0f)
{
  this->updateModel();
}

const glm::vec2 &ngyn::Transform::position()
{
  return _position;
}

const glm::vec2 &ngyn::Transform::size()
{
  return _size;
}

const glm::vec2 &ngyn::Transform::scale()
{
  return _scale;
}

const float &ngyn::Transform::rotation()
{
  return _rotation;
}

const Transform::LayerMode &ngyn::Transform::layerMode()
{
  return _layerMode;
}

const int &ngyn::Transform::layer()
{
  return _layer;
}

const glm::mat4 &ngyn::Transform::model()
{
  return _model;
}

const float &ngyn::Transform::zIndex()
{
  return _zIndex;
}

void ngyn::Transform::setPosition(const glm::vec2 &position)
{
  if(_position == position) return;

  _position = position;
  updateModel();
}

void ngyn::Transform::setSize(const glm::vec2 &size)
{
  if(_size == size) return;

  _size = size;
  updateModel();
}

void ngyn::Transform::setScale(const glm::vec2 &scale)
{
  if(_scale == scale) return;

  _scale = scale;
  updateModel();
}

void ngyn::Transform::setRotation(const float &rotation)
{
  if(_rotation == rotation) return;

  _rotation = rotation;
  updateModel();
}

void ngyn::Transform::setLayerMode(const LayerMode &layerMode)
{
  if(_layerMode == layerMode) return;

  _layerMode = layerMode;
  updateModel();
}

void ngyn::Transform::setLayer(const int &layer)
{
  if(_layer == layer) return;

  _layer = layer;
  updateModel();
}

void ngyn::Transform::moveBy(const glm::vec2 &velocity)
{
  if(velocity == glm::vec2(0.0f)) return;
  _position += velocity;
  updateModel();
}

void ngyn::Transform::resizeBy(const glm::vec2 &size)
{
  if(size == glm::vec2(0.0f)) return;
  _size += size;
  updateModel();
}

void ngyn::Transform::scaleBy(const glm::vec2 &scale)
{
  if(scale == glm::vec2(0.0f)) return;
  _scale += scale;
  updateModel();
}

void ngyn::Transform::rotateBy(const float &deg)
{
  if(deg == 0) return;
  _rotation += deg;
  updateModel();
}

void Transform::updateModel()
{
  _model = glm::mat4(1.0f);
  _zIndex = _layer;

  if(_layerMode == LayerMode::Dynamic)
  {
    float yOffset = (_position.y + _size.y) * 0.00001f;
    _zIndex += yOffset;
  }

  _model = glm::translate(_model, glm::vec3(_position, 0.0f));

  _model = glm::translate(_model, glm::vec3(_size * 0.5f, 0.0f));
  _model = glm::rotate(_model, glm::radians(_rotation), glm::vec3(0.0f, 0.0f, 1.0f));
  _model = glm::translate(_model, glm::vec3(_size * -0.5f, 0.0f));

  _model = glm::scale(_model, glm::vec3(_size * _scale, 0.0f));
}