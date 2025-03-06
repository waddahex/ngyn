#pragma once

#include "renderer.hpp"
#include "../resources/resources_manager.hpp"
#include "../resources/texture.hpp"
#include "../resources/shader.hpp"
#include "../util/logger.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace ngyn
{
  struct QuadInstanceData
  {
    glm::mat4 mvp;        // 64 bytes
    glm::vec4 texCoords1; // 16 bytes
    glm::vec4 texCoords2; // 16 bytes
    glm::vec4 color;      // 16 bytes
    int textureID;        // 4 bytes
    int padding[3];       // 12 bytes
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