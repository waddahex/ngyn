#include <ngyn/core/window.hpp>
#include <ngyn/core/time.hpp>
#include <ngyn/core/input.hpp>
#include <ngyn/util/logger.hpp>
#include <ngyn/resources/shader.hpp>
#include <ngyn/resources/texture.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <thread>

using namespace ngyn;

const char *vShaderData = R"(
#version 330 core
layout (location = 0) in vec2 aPos;

uniform vec4 texCoords1;
uniform vec4 texCoords2;
uniform mat4 model;
uniform mat4 projection;

out vec2 texCoords;

void main()
{
  gl_Position = projection * model * vec4(aPos, 0.0, 1.0);

  if(gl_VertexID == 0) texCoords = texCoords1.xy;
  else if(gl_VertexID == 1) texCoords = texCoords1.zw;
  else if(gl_VertexID == 2) texCoords = texCoords2.xy;
  else texCoords = texCoords2.zw;
}
)";

const char *fShaderData = R"(
#version 330 core
out vec4 FragColor;

in vec2 texCoords;
uniform sampler2D sample;

void main()
{
  FragColor = texture(sample, texCoords);
}
)";

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

  Shader shader({.vShaderData = vShaderData, .fShaderData = fShaderData});

  float vertices[] = {
    0.0f, 0.0f, // bottom-left
    0.0f, 1.0f, // top-left
    1.0f, 1.0f, // top-right
    1.0f, 0.0f, // bottom-right
  };

  GLuint indices[] = {
    0, 1, 2,
    0, 2, 3,
  };

  GLuint VAO, VBO, EBO;

  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) * sizeof(float), vertices, GL_STATIC_DRAW);

  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices) * sizeof(GLuint), indices, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);

  Texture texture({
    .image = "c:/dev/ngyn/data/textures/idle.png",
    .filtering = TextureFiltering::Nearest
  });

  texture.bind();
  shader.setInt("sample", 0);

  glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(window.resolution.x), static_cast<float>(window.resolution.y), 0.0f, 1.0f, -1.0f);
  glm::mat4 model(1.0f);

  model = glm::scale(model, glm::vec3(512.0f, 512.0f, 0.0f));

  shader.use();

  shader.setMat4("projection", projection);
  shader.setMat4("model", model);

  int frames = 9;
  int currentFrame = 0;
  float frameSize = 32.0f;

  while(window.isOpen())
  {
    window.handleEvents();
    time.update();
    input.update(window);
    window.clear();

    if(input.pressed("KEY_F1"))
    {
      currentFrame == frames ? currentFrame = 0 : currentFrame++;
    }

    auto texCoords = getTexCoordinates(
      static_cast<float>(texture.width),
      static_cast<float>(texture.height),
      currentFrame * frameSize,
      0.0f,
      32.0f,
      32.0f
    );
    shader.setVec4("texCoords1", texCoords.first);
    shader.setVec4("texCoords2", texCoords.second);

    glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);

    window.swapBuffers();
  }
}