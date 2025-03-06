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

  const char *vShaderData =
R"(
  #version 460 core
  layout (location = 0) in vec2 aPos;

  uniform vec4 texCoords1;
  uniform vec4 texCoords2;

  out vec2 texCoords;

  void main()
  {
    gl_Position = vec4(aPos, 0.0, 1.0);

    if(gl_VertexID == 0) texCoords = texCoords1.xy;
    else if(gl_VertexID == 1) texCoords = texCoords1.zw;
    else if(gl_VertexID == 2) texCoords = texCoords2.xy;
    else texCoords = texCoords2.zw;
  }
)";

  const char *fShaderData =
R"(
  #version 460 core
  out vec4 FragColor;

  in vec2 texCoords;
  uniform sampler2D uTexture;

  void main()
  {
    // FragColor = vec4(1.0, 1.0, 1.0, 1.0);
    FragColor = texture(uTexture, texCoords);
  }
)";

int main()
{
  logger.setFormat("$T");

  Window window({
    .monitor = 1
  });

  Time time;
  Input input;

  // Font *font = ResourcesManager::addResource<Font>("minecraft_font", Font({
  //   .path = "data/fonts/minecraft.ttf",
  //   .name = "minecraft_font",
  //   .size = 128, .pixelated = true
  // }));

  // auto charA = font->characters['A'];

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
    .path = "data/textures/idle.png",
    .pixelated = true
  }));

  Shader *shader = ResourcesManager::addResource<Shader>("shader", Shader({
    .vShaderData = vShaderData,
    .fShaderData = fShaderData
  }));

  auto texCoords = getTexCoordinates(
    static_cast<float>(catIdle->width),
    static_cast<float>(catIdle->height),
    0.0f,
    0.0f,
    static_cast<float>(catIdle->width),
    static_cast<float>(catIdle->height)
  );

  std::vector<float> vertices = {
    0.0f, 0.0f, // bottom-left
    0.0f, 1.0f, // top-left
    1.0f, 1.0f, // top-right
    1.0f, 0.0f, // bottom-right
  };

  std::vector<GLuint> indices = {
    0, 1, 2,
    0, 2, 3
  };

  GLint size = 2;
  GLsizei stride = sizeof(float) * 2;
  GLenum drawMode = GL_TRIANGLES;

  GLuint VAO, VBO, EBO;

  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(
    GL_ARRAY_BUFFER,
    vertices.size() * sizeof(float),
    vertices.data(),
    GL_STATIC_DRAW
  );

  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(
    GL_ELEMENT_ARRAY_BUFFER,
    indices.size() * sizeof(GLuint),
    indices.data(),
    GL_STATIC_DRAW
  );

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, size, GL_FLOAT, GL_FALSE, stride, (void*)0);

  shader->use();
  shader->setInt("uTexture", catIdle->index);
  shader->setVec4("texCoords1", texCoords.first);
  shader->setVec4("texCoords2", texCoords.second);
  catIdle->bind();

  while(window.isOpen())
  {
    window.handleEvents();
    time.update();
    input.update(window);
    window.clear();

    glDrawElements(drawMode, indices.size(), GL_UNSIGNED_INT, 0);

    window.swapBuffers();
  }
}