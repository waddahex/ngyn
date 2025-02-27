#include <iostream>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

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

int main()
{
  glfwInit();

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window = glfwCreateWindow(1280, 720, "Test", nullptr, nullptr);

  glfwMakeContextCurrent(window);

  gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

  glClearColor(0.1f, 0.2f, 0.3f, 1.0f);

  GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vShader, 1, &vShaderData, nullptr);
  glCompileShader(vShader);

  int success;
  char infoLog[1024];

  glGetShaderiv(vShader, GL_COMPILE_STATUS, &success);

  if(!success)
  {
    glGetShaderInfoLog(vShader, sizeof(infoLog), nullptr, infoLog);
    std::cout << "Vertex shader compile error" << infoLog << std::endl;
  }

  GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fShader, 1, &fShaderData, nullptr);
  glCompileShader(fShader);

  glGetShaderiv(fShader, GL_COMPILE_STATUS, &success);
  if(!success)
  {
    glGetShaderInfoLog(fShader, sizeof(infoLog), nullptr, infoLog);
    std::cout << "Fragment shader compile error" << infoLog << std::endl;
  }

  GLuint shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vShader);
  glAttachShader(shaderProgram, fShader);
  glLinkProgram(shaderProgram);

  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if(!success)
  {
    glGetShaderInfoLog(shaderProgram, sizeof(infoLog), nullptr, infoLog);
    std::cout << "Shader program link error" << infoLog << std::endl;
  }

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

  GLuint VAO;
  GLuint VBO;
  GLuint EBO;

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

  glUseProgram(shaderProgram);

  while(!glfwWindowShouldClose(window))
  {
    glfwPollEvents();
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
  }

  glfwDestroyWindow(window);
  glfwTerminate();
}