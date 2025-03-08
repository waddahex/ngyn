#pragma once

#include "../../ngynpch.hpp"
#include "../resources/texture.hpp"

namespace ngyn
{

  class Frame
  {
    public:
    struct CreateInfo
    {
      std::shared_ptr<Texture> texture = nullptr;
      glm::vec2 offset = glm::vec2(0.0f);
      glm::vec2 size = glm::vec2(0.0f);
      glm::vec4 color = glm::vec4(1.0f);
      glm::bvec2 flip = glm::bvec2(false, false);
    };

    public:
    Frame(void) = default;
    Frame(CreateInfo createInfo);

    // Setters
    void setTexture(const std::shared_ptr<Texture> &texture);
    void setColor(const glm::vec4 &color);
    void setFlip(const glm::bvec2 &flip);
    void setOffset(const glm::vec2 &offset);
    void setSize(const glm::vec2 &size);

    // Getters
    const std::shared_ptr<Texture> &texture();
    const glm::vec2 &offset();
    const glm::vec2 &size();
    const glm::vec4 &color();
    const glm::bvec2 &flip();
    const glm::vec4 &texCoords1();
    const glm::vec4 &texCoords2();

    private:
    // CreateInfo properties
    std::shared_ptr<Texture> _texture;
    glm::vec2 _offset;
    glm::vec2 _size;
    glm::vec4 _color;
    glm::bvec2 _flip;

    // Other properties
    glm::vec4 _texCoords1;
    glm::vec4 _texCoords2;

    void updateTexCoords();
  };
};