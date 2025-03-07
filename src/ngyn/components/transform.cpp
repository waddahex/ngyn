#include "transform.hpp"

using namespace ngyn;

Transform::Transform(TransformCreateInfo createInfo)
{
  this->position = createInfo.position;
  this->size = createInfo.size;
  this->rotation = createInfo.rotation;
  this->layer = createInfo.layer;

  this->model = glm::mat4(1.0f);
  this->zIndex = 0.0f;

  this->updateModel();
}

void Transform::updateModel()
{
  this->model = glm::mat4(1.0f);

  this->model = glm::translate(this->model, this->position);
  this->model = glm::scale(this->model, this->size);
}