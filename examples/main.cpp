#include <iostream>

#include <resources/shader.hpp>
#include <core/engine.hpp>
#include <util/logger.hpp>

const char *vShaderData = R"(
  #version 330 core
  layout (location = 0) in vec2 aPos;

  void main()
  {
    gl_Position = vec4(aPos, 0.0, 1.0);
  }
)";

const char *fShaderData = R"(
  #version 330 core

  out vec4 FragColor;

  void main()
  {
    FragColor = vec4(1.0, 1.0, 1.0, 1.0);
  }
)";

class Game : public ngyn::Engine
{
  public:
  Game(void) = default;

  ngyn::Shader shader;
  GLuint VAO;

  void onSetup()
  {
    this->shader = ngyn::Shader(ngyn::ShaderCreateInfo{
      .vShaderData = vShaderData,
      .fShaderData = fShaderData
    });

    float vertices[] = {
      0.0f, 0.0f, // bottom-left
      1.0f, 1.0f, // top-right
      0.0f, 1.0f, // top-left
      1.0f, 0.0f  // bottom-right
    };

    GLuint indices[] = {
      0, 1, 2,
      0, 1, 3
    };

    GLuint VBO;
    GLuint EBO;

    glGenVertexArrays(1, &this->VAO);
    glBindVertexArray(this->VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) * sizeof(float), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices) * sizeof(GLuint), indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);
  };

  void onUpdate()
  {
  };

  void onRender()
  {
    shader.use();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  };
};

int main()
{
  std::string test = "test";

  ngyn::logger.setFormat("$T");
  LOGGER_DEBUG("{}, {}, {}", false, 1, "true"); // 0, 1
  ngyn::logger.setFormat("dd/MM/yyyy HH:mm:ss $T");
  LOGGER_ERROR("{1}, {0}", "first", "second"); // 1, 0
  ngyn::logger.setFormat("HH:mm:ss");
  ngyn::logger.warn("{}, {1}, {0}", "first", "second"); // 0, 0, 1

  // LOGGER_DEBUG("Template string", "first", "second", "third", "fourth");
  // Game game;
  // game.run();
}