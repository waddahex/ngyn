#include "mouse.hpp"

using namespace ngyn;

glm::vec2 Mouse::_client;
glm::vec2 Mouse::_world;

const glm::vec2 &ngyn::Mouse::client()
{
  return _client;
}

void ngyn::Mouse::setClient(const glm::vec2 &position)
{
  _client = position;
}

const glm::vec2 &ngyn::Mouse::world()
{
  return _world;
}

void ngyn::Mouse::setWorld(const glm::vec2 &position)
{
  _world = position;
}