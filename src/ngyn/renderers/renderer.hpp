#pragma once

#include <glad/glad.h>
#include <vector>
#include "../resources/shader.hpp"

namespace ngyn
{
  template<typename T>
  class Renderer
  {
    public:
    Renderer(void) = default;

    void render();
    void setup();
    virtual void onSetup() = 0;
    virtual void onRender() = 0;

    std::vector<T> instancesData;

    protected:
    std::vector<float> vertices;
    std::vector<GLuint> indices;
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
    GLuint SSBO;
    size_t instancesCount;

    GLint size;
    GLsizei stride;
    GLenum drawMode;

    Shader *shader = nullptr;
  };

  template <typename T>
  inline void Renderer<T>::render()
  {
    if(this->instancesData.empty())
    {
      return;
    }

    this->onRender();

    glBindVertexArray(this->VAO);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, this->SSBO);

    if(this->instancesCount != this->instancesData.size())
    {
      glBufferData(
        GL_SHADER_STORAGE_BUFFER,
        this->instancesData.size()* sizeof(T),
        this->instancesData.data(),
        GL_DYNAMIC_DRAW
      );
      
      this->instancesCount = this->instancesData.size();
    }
    else
    {
      glBufferSubData(
        GL_SHADER_STORAGE_BUFFER,
        0,
        this->instancesData.size() * sizeof(T),
        this->instancesData.data()
      );
    }

    glDrawElementsInstanced(
      this->drawMode,
      static_cast<GLsizei>(this->indices.size()),
      GL_UNSIGNED_INT,
      0,
      static_cast<GLsizei>(this->instancesData.size())
    );

    glBindVertexArray(0);
  }

  template <typename T>
  inline void Renderer<T>::setup()
  {
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(
      GL_ARRAY_BUFFER,
      this->vertices.size() * sizeof(float),
      this->vertices.data(),
      GL_STATIC_DRAW
    );

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(
      GL_ELEMENT_ARRAY_BUFFER,
      this->indices.size() * sizeof(GLuint),
      this->indices.data(),
      GL_STATIC_DRAW
    );

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, this->size, GL_FLOAT, GL_FALSE, this->stride, (void*)0);

    glGenBuffers(1, &SSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, SSBO);

    this->onSetup();
  }
};