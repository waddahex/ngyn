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

void Sprite::update()
{
  ASSERT(_renderer.get(), "Invalid renderer");
  ASSERT(_camera.get(), "Invalid camera");

  auto renderer = _renderer.get();
  auto camera = _camera.get();

  _instanceIndex = renderer->addInstance(QuadInstanceData{
    .mvp = camera->view() * camera->projection() * transform.model(),
    .texCoords1 = frame.texCoords1(),
    .texCoords2 = frame.texCoords2(),
    .color = frame.color(),
    .textureID = frame.texture().get()->index,
    .zIndex = transform.zIndex()
  });
}

void ngyn::Sprite::instantiate()
{
  update();
}