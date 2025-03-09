#include "sprite.hpp"

using namespace ngyn;

Sprite::Sprite(CreateInfo createInfo) :
  frame(createInfo.frame),
  transform(createInfo.transform),
  _instanceIndex(-1),
  _renderer(createInfo.renderer),
  _camera(createInfo.camera)
{
}

const int &ngyn::Sprite::instanceIndex()
{
  return _instanceIndex;
}

std::shared_ptr<QuadRenderer> ngyn::Sprite::renderer()
{
  return _renderer;
}

std::shared_ptr<Camera> ngyn::Sprite::camera()
{
  return _camera;
}

void ngyn::Sprite::setRenderer(std::shared_ptr<QuadRenderer> renderer)
{
  _renderer = renderer;
}

void ngyn::Sprite::setCamera(std::shared_ptr<Camera> camera)
{
  _camera = camera;
}

void Sprite::moveBy(const glm::vec2 &velocity)
{
  if(velocity == glm::vec2(0.0f)) return;
  transform.setPosition(transform.position() + velocity);
}

void Sprite::resizeBy(const glm::vec2 &size)
{
  if(size == glm::vec2(0.0f)) return;
  transform.setSize(transform.size() + size);
}

void Sprite::scaleBy(const glm::vec2 &scale)
{
  if(scale == glm::vec2(0.0f)) return;
  transform.setScale(transform.scale() + scale);
}

void Sprite::rotateBy(const float &deg)
{
  if(deg == 0) return;
  transform.setRotation(transform.rotation() + deg);
}

void Sprite::update()
{
  ASSERT(_renderer.get(), "Invalid renderer");
  ASSERT(_instanceIndex >= 0, "Invalid instance");

  auto renderer = _renderer.get();
  auto camera = _camera.get();

  renderer->setInstance(_instanceIndex, getData());
}

void ngyn::Sprite::instantiate()
{
  ASSERT(_renderer.get(), "Invalid renderer");

  auto renderer = _renderer.get();

  _instanceIndex = renderer->addInstance(getData());
}

QuadInstanceData Sprite::getData()
{
  ASSERT(_camera.get(), "Invalid camera");
  auto camera = _camera.get();

  return QuadInstanceData{
    .mvp = camera->view() * camera->projection() * transform.model(),
    .texCoords1 = frame.texCoords1(),
    .texCoords2 = frame.texCoords2(),
    .color = frame.color(),
    .textureID = frame.texture().get()->index,
    .zIndex = transform.zIndex()
  };
}