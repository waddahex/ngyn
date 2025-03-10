#pragma once

#include "../../ngynpch.hpp"

#include "texture.hpp"
#include "../util/logger.hpp"

namespace ngyn
{
  struct FontCreateInfo
  {
    std::string path;
    std::string name;
    int size = 16;
    bool pixelated = false;
  };

  struct FontCharacter
  {
    float xOffset;
    float yOffset;
    float width;
    float height;
    float bearingX;
    float bearingY;
    float advanceWidth;
  };

  class Font
  {
    public:
    Font(void) = default;
    Font(FontCreateInfo createInfo);

    std::weak_ptr<Texture> texture;
    float maxHeight;
    float spaceAdvance;

    std::unordered_map<unsigned char, FontCharacter> characters;

    void destroy();
  };
};