#include <ngyn/ngyn.hpp>

using namespace ngyn;

int main()
{
  logger.setFormat("$T");

  Window window({
    .monitor = 1
  });

  Time time;
  Input input;
  QuadRenderer renderer;
  renderer.setup();

  auto texture = ResourcesManager::addResource<Texture>("arrows", Texture(TextureCreateInfo{
    .image = "data/textures/arrows.png",
    .filtering = TextureFiltering::Nearest
  }));

  Frame frame{{
    .texture = texture,
    .offset = glm::vec2(0.0f),
    .size = glm::vec2(32.0f),
    .flip = glm::bvec2(false, false)
  }};

  Frame frame2{{
    .texture = texture,
    .offset = glm::vec2(32.0f, 32.0f),
    .size = glm::vec2(32.0f),
    .flip = glm::bvec2(false, false)
  }};

  Transform transform{{
    .position = glm::vec2(500.0f),
    .size = glm::vec2(128.0f),
    .layerMode = Transform::LayerMode::Dynamic,
    .layer = 1
  }};

  Transform transform2{{
    .position = glm::vec2(500.0f),
    .size = glm::vec2(128.0f),
    .layerMode = Transform::LayerMode::Dynamic,
    .layer = 1
  }};

  glm::mat4 projection = glm::ortho(
    0.0f,
    static_cast<float>(window.resolution.x),
    static_cast<float>(window.resolution.y),
    0.0f,
    -1.0f,
    1.0f
  );

  int instance1 = renderer.addInstance({
    .mvp = projection * transform.model(),
    .texCoords1 = frame.texCoords1(),
    .texCoords2 = frame.texCoords2(),
    .color = frame.color(),
    .textureID = frame.texture().get()->index,
    .zIndex = transform.zIndex()
  });

  int instance2 = renderer.addInstance({
    .mvp = projection * transform2.model(),
    .texCoords1 = frame2.texCoords1(),
    .texCoords2 = frame2.texCoords2(),
    .color = frame2.color(),
    .textureID = frame2.texture().get()->index,
    .zIndex = transform2.zIndex()
  });

  float speed = 300.0f;

  while(window.isOpen())
  {
    window.handleEvents();
    time.update();
    input.update(window);

    glm::vec2 velocity(0.0f);

    if(input.held("KEY_A"))
    {
      velocity.x -= speed * time.deltaTime;
    }

    if(input.held("KEY_D"))
    {
      velocity.x += speed * time.deltaTime;
    }

    if(input.held("KEY_W"))
    {
      velocity.y -= speed * time.deltaTime;
    }

    if(input.held("KEY_S"))
    {
      velocity.y += speed * time.deltaTime;
    }

    float rotation = 0.0f;
    if(input.held("KEY_Q"))
    {
      rotation += speed * time.deltaTime;
    }

    if(input.held("KEY_E"))
    {
      rotation -= speed * time.deltaTime;
    }

    glm::vec2 scale(0.0f);

    if(input.held("KEY_R"))
    {
      scale.x -= 10.0f * time.deltaTime;
    }

    if(input.held("KEY_F"))
    {
      scale.x += 10.0f * time.deltaTime;
    }

    if(input.held("KEY_T"))
    {
      scale.y -= 10.0f * time.deltaTime;
    }

    if(input.held("KEY_G"))
    {
      scale.y += 10.0f * time.deltaTime;
    }

    if(velocity != glm::vec2(0.0f) || rotation != 0.0f || scale != glm::vec2(0.0f))
    {
      auto currentPosition = transform2.position();
      transform2.setPosition(currentPosition += velocity);

      auto currentRotation = transform2.rotation();
      transform2.setRotation(currentRotation += rotation);

      auto currentScale = transform2.scale();
      transform2.setScale(currentScale += scale);
  
      auto instanceData = renderer.getInstance(instance2);
      instanceData.mvp = projection * transform2.model();
      instanceData.zIndex = transform2.zIndex();
      renderer.setInstance(instance2, instanceData);
    }

    window.clear();

    renderer.render();

    window.swapBuffers();
  }
}