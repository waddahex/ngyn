#include "frame.hpp"

using namespace ngyn;

Frame::Frame(CreateInfo createInfo) :
  _texture(createInfo.texture),
  _offset(createInfo.offset),
  _size(createInfo.size),
  _color(createInfo.color),
  _flip(createInfo.flip),
  _texCoords1(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)),
  _texCoords2(glm::vec4(1.0f, 1.0f, 1.0f, 0.0f))
{
  ASSERT(_texture, "Texture was not initialized");
  ASSERT(_texture.get(), "Invalid texture");

  if(_size == glm::vec2(0.0f))
  {
    _size = glm::vec2(_texture.get()->width, _texture.get()->height);
  }

  updateTexCoords();
}

void ngyn::Frame::setTexture(const std::shared_ptr<Texture> &texture)
{
  ASSERT(texture, "Texture was not initialized");
  ASSERT(texture.get(), "Invalid texture");

  _texture = texture;
  updateTexCoords();
}

void Frame::setColor(const glm::vec4 &color)
{
  _color = color;
}

void Frame::setOffset(const glm::vec2 &offset)
{
  _offset = offset;
  updateTexCoords();
}

void Frame::setSize(const glm::vec2 &size)
{
  _size = size;
  updateTexCoords();
}

const std::shared_ptr<Texture> &ngyn::Frame::texture()
{
  return _texture;
}

const glm::vec2 &ngyn::Frame::offset()
{
  return _offset;
}

const glm::vec2 &ngyn::Frame::size()
{
  return _size;
}

const glm::vec4 &ngyn::Frame::color()
{
  return _color;
}

const glm::bvec2 &ngyn::Frame::flip()
{
  return _flip;
}

const glm::vec4 &ngyn::Frame::texCoords1()
{
  return _texCoords1;
}

const glm::vec4 &ngyn::Frame::texCoords2()
{
  return _texCoords2;
}

void Frame::setFlip(const glm::bvec2 &flip)
{
  _flip = flip;
  updateTexCoords();
}

void Frame::updateTexCoords()
{
  auto texturePtr = _texture.get();

  glm::vec2 bottomLeft(_offset.x / texturePtr->width, _offset.y / texturePtr->height);
  glm::vec2 topLeft(_offset.x / texturePtr->width, (_offset.y + _size.y) / texturePtr->height);
  glm::vec2 topRight((_offset.x + _size.x) / texturePtr->width, (_offset.y + _size.y) / texturePtr->height);
  glm::vec2 bottomRight((_offset.x + _size.x) / texturePtr->width, _offset.y / texturePtr->height);

  if(_flip.x && _flip.y)
  {
    _texCoords1 = glm::vec4(topRight, bottomRight);
    _texCoords2 = glm::vec4(bottomLeft, topLeft);

    return;
  }

  if(_flip.x)
  {
    _texCoords1 = glm::vec4(topRight, bottomRight);
    _texCoords2 = glm::vec4(bottomLeft, topLeft);

    return;
  }

  if(_flip.y)
  {
    _texCoords1 = glm::vec4(topLeft, bottomLeft);
    _texCoords2 = glm::vec4(bottomRight, topRight);

    return;
  }

  _texCoords1 = glm::vec4(bottomLeft, topLeft);
  _texCoords2 = glm::vec4(topRight, bottomRight);
}