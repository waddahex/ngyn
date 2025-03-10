#pragma once

#include "../../ngynpch.hpp"

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
    int addInstance(const T &data);
    void removeInstance(int index);
    T getInstance(int index);
    void setInstance(int index, const T &data);
    
    virtual void onSetup() = 0;
    virtual void onRender() = 0;

    int getInstancesCount();
    
    protected:
    size_t instancesCount;
    std::vector<float> vertices;
    std::vector<GLuint> indices;
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
    GLuint SSBO;

    GLint size;
    GLsizei stride;
    GLenum drawMode;

    std::weak_ptr<Shader> shader;

    std::vector<T> instancesData;
    std::vector<T> orderedData;
    std::vector<int> unusedInstances;
  };

  template <typename T>
  inline void Renderer<T>::render()
  {
    this->onRender();

    if(this->orderedData.empty())
    {
      return;
    }

    glBindVertexArray(this->VAO);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, this->SSBO);

    if(this->instancesCount != this->orderedData.size())
    {
      glBufferData(
        GL_SHADER_STORAGE_BUFFER,
        this->orderedData.size()* sizeof(T),
        this->orderedData.data(),
        GL_DYNAMIC_DRAW
      );
      
      this->instancesCount = this->orderedData.size();
    }
    else
    {
      glBufferSubData(
        GL_SHADER_STORAGE_BUFFER,
        0,
        this->orderedData.size() * sizeof(T),
        this->orderedData.data()
      );
    }

    glDrawElementsInstanced(
      this->drawMode,
      static_cast<GLsizei>(this->indices.size()),
      GL_UNSIGNED_INT,
      0,
      static_cast<GLsizei>(this->orderedData.size())
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

  template <typename T>
  inline int Renderer<T>::addInstance(const T &data)
  {
    if(this->unusedInstances.empty())
    {
      this->instancesData.push_back(data);
      return static_cast<int>(this->instancesData.size() -1);
    }

    int index = this->unusedInstances[0];
    this->unusedInstances.erase(this->unusedInstances.begin());

    this->instancesData[index] = data;
    return index;
  }

  template <typename T>
  inline void Renderer<T>::removeInstance(int index)
  {
    if(index > this->instancesData.size() -1)
    {
      return;
    }

    this->unusedInstances.push_back(this->instancesData.size() -1);
  }

  template <typename T>
  inline T Renderer<T>::getInstance(int index)
  {
    if(index > this->instancesData.size() -1) return T();
    return this->instancesData[index];
  }

  template <typename T>
  inline void Renderer<T>::setInstance(int index, const T &data)
  {
    if(index > this->instancesData.size() -1) return;
    this->instancesData[index] = data;
  }

  template <typename T>
  inline int Renderer<T>::getInstancesCount()
  {
    return static_cast<int>(this->instancesData.size());
  }
};