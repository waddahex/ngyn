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
  _layer(createInfo.layer),
  _alignment(createInfo.alignment)
{
}

std::weak_ptr<Font> Text::font()
{
  return _font;
}

std::weak_ptr<Camera> Text::camera()
{
  return _camera;
}

std::weak_ptr<QuadRenderer> Text::renderer()
{
  return _renderer;
}

const glm::vec2 &Text::position()
{
  return _position;
}

const glm::vec2 &Text::size()
{
  return _size;
}

const float &Text::rotation()
{
  return _rotation;
}

const std::string &Text::value()
{
  return _value;
}

const Color &Text::color()
{
  return _color;
}

const Transform::LayerMode &Text::layerMode()
{
  return _layerMode;
}

const int &Text::layer()
{
  return _layer;
}

const Alignment &Text::alignment()
{
  return _alignment;
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



    Frame frame{{
      .texture = font->texture(),
      .offset = glm::vec2(character.xOffset, character.yOffset),
      .size = size,
      .color = _color,
    }};

    Transform transform{{
      .position = position,
      .size = size,
      .layerMode = _layerMode,
      .layer = _layer
    }};

    _indexes.push_back(renderer->addInstance(QuadInstanceData{
      .mvp = camera->view() * camera->projection() * transform.model(),
      .texCoords1 = frame.texCoords1(),
      .texCoords2 = frame.texCoords2(),
      .color = frame.color(),
      .textureID = frame.texture().lock() ? frame.texture().lock().get()->index() : -1,
      .zIndex = transform.zIndex(),
      .isText = 1
    }));

    offsetX += character.width + spacingX;
  }
}