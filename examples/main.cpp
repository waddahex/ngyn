#include <ngyn/core/window.hpp>
#include <ngyn/core/time.hpp>
#include <ngyn/core/input.hpp>
#include <ngyn/util/logger.hpp>
#include <ngyn/resources/shader.hpp>
#include <ngyn/resources/texture.hpp>
#include <ngyn/resources/font.hpp>
#include <ngyn/renderers/quad_renderer.hpp>

#include <ngyn/resources/resources_manager.hpp>

#include <glm/gtc/matrix_transform.hpp>

using namespace ngyn;

std::pair<glm::vec4, glm::vec4> getTexCoordinates(float textureWidth, float textureHeight, float x, float y, float width, float height)
{
  glm::vec2 bottomLeft(x / textureWidth, y / textureHeight);
  glm::vec2 topLeft(x / textureWidth, (y + height) / textureHeight);
  glm::vec2 topRight((x + width) / textureWidth, (y + height) / textureHeight);
  glm::vec2 bottomRight((x + width) / textureWidth, y / textureHeight);

  return std::make_pair(glm::vec4(bottomLeft, topLeft), glm::vec4(topRight, bottomRight));
}

int main()
{
  logger.setFormat("$T");

  Window window({
    .monitor = 1
  });

  Time time;
  Input input;

  Font *font = ResourcesManager::addResource<Font>("minecraft_font", Font({
    .path = "data/fonts/minecraft.ttf",
    .name = "minecraft_font",
    .size = 128, .pixelated = true
  }));

  auto charA = font->characters['A'];

  QuadRenderer quadRenderer;
  quadRenderer.setup();

  glm::mat4 projection = glm::ortho(
    0.0f,
    static_cast<float>(window.resolution.x),
    static_cast<float>(window.resolution.y),
    0.0f,
    -1.0f,
    1.0f
  );

  glm::mat4 model(1.0f);
  model = glm::scale(model, glm::vec3(128.0f, 128.0f, 0.0f));

  Texture *catIdle = ResourcesManager::addResource<Texture>("cat", Texture({
    .image = "data/textures/idle.png",
    .filtering = TextureFiltering::Nearest
  }));

  auto texCoords = getTexCoordinates(
    static_cast<float>(font->texture->width),
    static_cast<float>(font->texture->height),
    charA.xOffset,
    charA.yOffset,
    charA.width,
    charA.height
  );

  quadRenderer.instancesData.push_back(QuadInstanceData{
    .mvp = projection * model,
    .texCoords1 = texCoords.first,
    .texCoords2 = texCoords.second,
    .color = glm::vec4(0.1f, 0.2f, 0.3f, 0.3f),
    .textureID = font->texture->index
  });

  while(window.isOpen())
  {
    window.handleEvents();
    time.update();
    input.update(window);
    window.clear();

    quadRenderer.render();

    window.swapBuffers();
  }
}