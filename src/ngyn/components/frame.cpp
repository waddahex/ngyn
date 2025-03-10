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
  if(_size == glm::vec2(0.0f) && _texture.lock())
  {
    auto texture = _texture.lock().get();
    _size = glm::vec2(texture->width, texture->height);
  }

  updateTexCoords();
}

void ngyn::Frame::setTexture(const std::weak_ptr<Texture> &texture)
{
  _texture = texture;
  updateTexCoords();
}

void Frame::setColor(const Color &color)
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

const std::weak_ptr<Texture> &ngyn::Frame::texture()
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

const Color &ngyn::Frame::color()
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
  if(!_texture.lock()) return;

  auto texture = _texture.lock().get();

  glm::vec2 bottomLeft(_offset.x / texture->width, _offset.y / texture->height);
  glm::vec2 topLeft(_offset.x / texture->width, (_offset.y + _size.y) / texture->height);
  glm::vec2 topRight((_offset.x + _size.x) / texture->width, (_offset.y + _size.y) / texture->height);
  glm::vec2 bottomRight((_offset.x + _size.x) / texture->width, _offset.y / texture->height);

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