#include "text.hpp"

using namespace ngyn;

Text::Text(CreateInfo createInfo) :
  _font(createInfo.font),
  _camera(createInfo.camera),
  _renderer(createInfo.renderer),
  _position(createInfo.position),
  _size(glm::vec2(0.0f)),
  _rotation(createInfo.rotation),
  _value(createInfo.value),
  _color(createInfo.color),
  _layerMode(createInfo.layerMode),
  _layer(createInfo.layer)
{
}

std::weak_ptr<Font> ngyn::Text::font()
{
  return _font;
}

std::weak_ptr<Camera> ngyn::Text::camera()
{
  return _camera;
}

std::weak_ptr<QuadRenderer> ngyn::Text::renderer()
{
  return _renderer;
}

const glm::vec2 &ngyn::Text::position()
{
  return _position;
}

const glm::vec2 &ngyn::Text::size()
{
  return _size;
}

const float &ngyn::Text::rotation()
{
  return _rotation;
}

const std::string &ngyn::Text::value()
{
  return _value;
}

const Color &ngyn::Text::color()
{
  return _color;
}

const Transform::LayerMode &ngyn::Text::layerMode()
{
  return _layerMode;
}

const int &ngyn::Text::layer()
{
  return _layer;
}

void Text::instantiate()
{
  ASSERT(_font.lock(), "Invalid font");
  ASSERT(_camera.lock(), "Invalid camera");
  ASSERT(_renderer.lock(), "Invalid renderer");

  auto font = _font.lock().get();
  auto camera = _camera.lock().get();
  auto renderer = _renderer.lock().get();

  float offsetX = 0.0f;
  float spacingX = 1.0f;

  // TODO: Apply rotation to initial model, needs total size
  for(auto c : _value)
  {
    if(static_cast<int>(c) == static_cast<int>(' '))
    {
      offsetX += font->spaceAdvance();
      continue;
    }

    auto character = font->getChar(c);

    float yFiller = font->maxHeight() - character.bearingY;

    glm::vec2 position = glm::vec2(_position.x + offsetX, _position.y + yFiller);
    glm::vec2 size = glm::vec2(character.width, character.height);

    Sprite sprite{Sprite::CreateInfo{
      .frame = {
        .texture = font->texture(),
        .offset = glm::vec2(character.xOffset, character.yOffset),
        .size = size,
        .color = _color,
      },
      .transform = Transform::CreateInfo{
        .position = position,
        .size = size,
        .layerMode = _layerMode,
        .layer = _layer
      },
      .camera = _camera
    }};

    auto qid = sprite.getData();
    qid.isText = 1;

    _indexes.push_back(renderer->addInstance(std::move(qid)));

    offsetX += character.width + spacingX;
  }
}