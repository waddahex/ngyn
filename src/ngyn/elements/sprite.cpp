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

std::weak_ptr<QuadRenderer> ngyn::Sprite::renderer()
{
  return _renderer;
}

std::weak_ptr<Camera> ngyn::Sprite::camera()
{
  return _camera;
}

void ngyn::Sprite::setRenderer(std::weak_ptr<QuadRenderer> renderer)
{
  _renderer = renderer;
}

void ngyn::Sprite::setCamera(std::weak_ptr<Camera> camera)
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
  ASSERT(_renderer.lock(), "Invalid renderer");
  ASSERT(_camera.lock(), "Invalid camera");
  ASSERT(_instanceIndex >= 0, "Invalid instance");

  auto renderer = _renderer.lock().get();
  auto camera = _camera.lock().get();

  renderer->setInstance(_instanceIndex, getData());
}

void ngyn::Sprite::instantiate()
{
  ASSERT(_renderer.lock(), "Invalid renderer");

  auto renderer = _renderer.lock().get();

  _instanceIndex = renderer->addInstance(getData());
}

QuadInstanceData Sprite::getData()
{
  ASSERT(_camera.lock(), "Invalid camera");
  auto camera = _camera.lock().get();

  return QuadInstanceData{
    .mvp = camera->view() * camera->projection() * transform.model(),
    .texCoords1 = frame.texCoords1(),
    .texCoords2 = frame.texCoords2(),
    .color = frame.color(),
    .textureID = frame.texture().lock() ? frame.texture().lock().get()->index() : -1,
    .zIndex = transform.zIndex()
  };
}