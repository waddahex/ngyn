#include "frame.hpp"

using namespace ngyn;

Frame::Frame(FrameCreateInfo createInfo) :
  texture(createInfo.texture),
  offset(createInfo.offset),
  size(createInfo.size),
  flip(createInfo.flip),
  texCoords1(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)),
  texCoords2(glm::vec4(1.0f, 1.0f, 1.0f, 0.0f))
{
  updateTexCoords();
}

void Frame::updateTexCoords()
{
  ASSERT(this->texture, "Texture was not initialized");
  ASSERT(this->texture.get(), "Invalid texture");

  auto texturePtr = this->texture.get();

  glm::vec2 bottomLeft(offset.x / texturePtr->width, offset.y / texturePtr->height);
  glm::vec2 topLeft(offset.x / texturePtr->width, (offset.y + size.y) / texturePtr->height);
  glm::vec2 topRight((offset.x + size.x) / texturePtr->width, (offset.y + size.y) / texturePtr->height);
  glm::vec2 bottomRight((offset.x + size.x) / texturePtr->width, offset.y / texturePtr->height);

  if(flip.x && flip.y)
  {
    this->texCoords1 = glm::vec4(topRight, bottomRight);
    this->texCoords2 = glm::vec4(bottomLeft, topLeft);
  }
  else if(flip.x)
  {
    this->texCoords1 = glm::vec4(topRight, bottomRight);
    this->texCoords2 = glm::vec4(bottomLeft, topLeft);
  }
  else if(flip.y)
  {
    this->texCoords1 = glm::vec4(topLeft, bottomLeft);
    this->texCoords2 = glm::vec4(bottomRight, topRight);
  }

  this->texCoords1 = glm::vec4(bottomLeft, topLeft);
  this->texCoords2 = glm::vec4(topRight, bottomRight);
}