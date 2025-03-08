#include "camera.hpp"

using namespace ngyn;

Camera::Camera(CreateInfo createInfo) :
  _position(createInfo.position),
  _resolution(createInfo.resolution),
  _near(createInfo.near),
  _far(createInfo.far),
  _projection(glm::mat4(1.0f)),
  _view(glm::mat4(1.0f))
{
  update();
}

const glm::vec2 &Camera::position()
{
  return _position;
}

const glm::vec2 &Camera::resolution()
{
  return _resolution;
}

const float &Camera::near()
{
  return _near;
}

const float &Camera::far()
{
  return _far;
}

const glm::mat4 &Camera::projection()
{
  return _projection;
}

const glm::mat4 &Camera::view()
{
  return _view;
}

void Camera::setPosition(const glm::vec2 &position)
{
  if(_position == position) return;
  _position = position;
  update();
}

void Camera::setResolution(const glm::vec2 &resolution)
{
  if(_resolution == resolution) return;
  _resolution = resolution;
  update();
}

void Camera::setNear(const float &near)
{
  if(_near == near) return;
  _near = near;
  update();
}

void Camera::setFar(const float &far)
{
  if(_far == far) return;
  _far = far;
  update();
}

void Camera::update()
{
  _projection = glm::ortho(0.0f, _resolution.x, _resolution.y, 0.0f, _near, _far);
  _view = glm::translate(glm::mat4(1.0f), -glm::vec3(_position, 0.0f));
}