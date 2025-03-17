#pragma once

#include "../../ngynpch.hpp"

#include "../resources/texture.hpp"
#include "../util/color.hpp"

namespace ngyn
{

  class Frame
  {
    public:
    enum Visibility
    {
      Hidden = 0,
      Visible = 1,
    };

    struct CreateInfo
    {
      std::weak_ptr<Texture> texture;
      unsigned int index = 0;
      glm::vec2 offset = glm::vec2(0.0f);
      glm::vec2 size = glm::vec2(0.0f);
      Color color = Color(255);
      glm::bvec2 flip = glm::bvec2(false, false);
      Visibility visibility = Visibility::Visible;
    };

    public:
    Frame(void) = default;
    Frame(CreateInfo createInfo);

    // Setters
    void setTexture(std::weak_ptr<Texture> texture);
    void setIndex(const unsigned int &index);
    void setOffset(const glm::vec2 &offset);
    void setSize(const glm::vec2 &size);
    void setColor(const Color &color);
    void setFlip(const glm::bvec2 &flip);
    void setVisibility(const Visibility &visible);

    // Getters
    std::weak_ptr<Texture> texture();
    const unsigned int &index();
    const glm::vec2 &offset();
    const glm::vec2 &size();
    const Color &color();
    const glm::bvec2 &flip();
    const glm::vec4 &texCoords1();
    const glm::vec4 &texCoords2();
    const Visibility &visibility();

    private:
    // CreateInfo properties
    std::weak_ptr<Texture> _texture;
    unsigned int _index;
    glm::vec2 _offset;
    glm::vec2 _size;
    Color _color;
    glm::bvec2 _flip;
    Visibility _visibility;

    // Other properties
    glm::vec4 _texCoords1;
    glm::vec4 _texCoords2;

    void updateTexCoords();
  };
};