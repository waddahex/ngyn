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

  auto texture = ResourcesManager::addResource<Texture>("arrow", Texture(TextureCreateInfo{
    .image = "data/textures/arrow.png",
    .filtering = TextureFiltering::Nearest
  }));

  Frame frame(FrameCreateInfo{
    .texture = texture,
    .offset = glm::vec2(0.0f),
    .size = glm::vec2(32.0f),
    .flip = glm::bvec2(false, false)
  });

  Transform transform(TransformCreateInfo{
    .position = glm::vec3(0.0f),
    .size = glm::vec3(128.0f, 128.0f, 0.0f),
  });

  glm::mat4 projection = glm::ortho(
    0.0f,
    static_cast<float>(window.resolution.x),
    static_cast<float>(window.resolution.y),
    0.0f,
    -1.0f,
    1.0f
  );

  renderer.addInstance({
    .mvp = projection * transform.model,
    .texCoords1 = frame.texCoords1,
    .texCoords2 = frame.texCoords2,
    .textureID = frame.texture.get()->index
  });

  std::vector<glm::vec2> test = {
    glm::vec2(1.0f),
    glm::vec2(2.0f)
  };

  LOGGER_DEBUG(glm::mat2(2.0f));
  LOGGER_DEBUG(glm::mat3(2.0f));
  LOGGER_DEBUG(glm::mat4(4.0f));

  while(window.isOpen())
  {
    window.handleEvents();
    time.update();
    input.update(window);
    window.clear();

    renderer.render();

    window.swapBuffers();
  }
}