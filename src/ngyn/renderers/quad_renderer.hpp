#pragma once

#include "../../ngynpch.hpp"

#include "renderer.hpp"
#include "../resources/resources_manager.hpp"
#include "../resources/texture.hpp"
#include "../resources/shader.hpp"
#include "../util/logger.hpp"

namespace ngyn
{
  struct QuadInstanceData
  {
    glm::mat4 mvp = glm::mat4(1.0f); // 64 bytes
    glm::vec4 texCoords1 = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f); // 16 bytes
    glm::vec4 texCoords2 = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f); // 16 bytes
    glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f); // 16 bytes
    int textureID = -1; // 4 bytes
    float zIndex = 0; // 4 bytes
    int padding[2]; // 8 bytes
  };

  class QuadRenderer : public Renderer<QuadInstanceData>
  {
    public:
    QuadRenderer();
    virtual void onSetup();
    virtual void onRender();

    std::string shaderName;
  };
};