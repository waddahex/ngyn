#include <ngyn/core/window.hpp>
#include <ngyn/core/time.hpp>
#include <ngyn/core/input.hpp>
#include <ngyn/util/logger.hpp>
#include <ngyn/util/random.hpp>
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

struct Transform
{
  glm::vec3 position;
  glm::vec3 size;
  glm::vec3 rotation;
};

struct Sprite
{
  glm::vec4 color;
};

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

  auto catCoords = getTexCoordinates(
    static_cast<float>(catIdle->width),
    static_cast<float>(catIdle->height),
    0.0f,
    0.0f,
    32.0f,
    32.0f
  );

  while(window.isOpen())
  {
    window.handleEvents();
    time.update();
    input.update(window);
    window.clear();

    quadRenderer.render();

    if(input.pressed("MOUSE_BUTTON_LEFT"))
    {
      int amount = 10000;

      for(size_t i = 0; i < amount; i++)
      {
        glm::vec2 size(8.0f);

        int x = random::getInteger(size.x, window.resolution.x - size.x * 2);
        int y = random::getInteger(size.y, window.resolution.y - size.y * 2);

        float r = random::getFloat(0.0f, 1.0f);
        float g = random::getFloat(0.0f, 1.0f);
        float b = random::getFloat(0.0f, 1.0f);

        glm::mat4 model(1.0f);
        model = glm::translate(model, glm::vec3(x, y, 0.0f));
        model = glm::scale(model, glm::vec3(size, 0.0f));

        quadRenderer.addInstance(QuadInstanceData{
          .mvp = projection * model,
          .color = glm::vec4(r, g, b, 1.0)
        });
      }
    }

    if(time.justUpdated)
    {
      window.setTitle(std::format("{:.2f} FPS | {:.2f} MS | {}", time.fps, time.ms, quadRenderer.getInstancesCount()));
    }

    window.swapBuffers();
  }
}