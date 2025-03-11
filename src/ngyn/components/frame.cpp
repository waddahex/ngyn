#include "frame.hpp"

using namespace ngyn;

Frame::Frame(CreateInfo createInfo) :
  _texture(createInfo.texture),
  _index(createInfo.index),
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
    _size = glm::vec2(texture->size().x, texture->size().y);
  }

  updateTexCoords();
}

void Frame::setTexture(std::weak_ptr<Texture> texture)
{
  ASSERT(texture.lock(), "Invalid texture");

  _texture = texture;
  updateTexCoords();
}

void Frame::setColor(const Color &color)
{
  _color = color;
}

void Frame::setIndex(const unsigned int &index)
{
  _index = index;
  updateTexCoords();
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

std::weak_ptr<Texture> Frame::texture()
{
  return _texture;
}

const unsigned int &Frame::index()
{
  return _index;
}

const glm::vec2 &Frame::offset()
{
  return _offset;
}

const glm::vec2 &Frame::size()
{
  return _size;
}

const Color &Frame::color()
{
  return _color;
}

const glm::bvec2 &Frame::flip()
{
  return _flip;
}

const glm::vec4 &Frame::texCoords1()
{
  return _texCoords1;
}

const glm::vec4 &Frame::texCoords2()
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

  glm::vec2 finalOffset = _offset;

  // Gets the offset from the index starting at one
  if(_index > 0)
  {
    int cols = texture->size().x / _size.x;
    int rows = texture->size().y / _size.y;

    int frames = cols * rows;

    int finalIndex = _index;

    if(_index > frames)
    {
      LOGGER_DEBUG(
        "Frame index out of bounds, using index 1. {} {}",
        _index,
        frames
      );

      finalIndex = 1;
    }

    int frameCol = finalIndex % cols == 0 ? cols : finalIndex % cols;
    int frameRow = std::ceil(finalIndex / static_cast<float>(cols));

    finalOffset = glm::vec2((frameCol - 1) * _size.x, (frameRow - 1) * _size.y);
  }

  glm::vec2 bottomLeft(finalOffset.x / texture->size().x, finalOffset.y / texture->size().y);
  glm::vec2 topLeft(finalOffset.x / texture->size().x, (finalOffset.y + _size.y) / texture->size().y);
  glm::vec2 topRight((finalOffset.x + _size.x) / texture->size().x, (finalOffset.y + _size.y) / texture->size().y);
  glm::vec2 bottomRight((finalOffset.x + _size.x) / texture->size().x, finalOffset.y / texture->size().y);

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