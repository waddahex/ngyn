#pragma once

#include "../../ngynpch.hpp"

#include "texture.hpp"
#include "../util/logger.hpp"

namespace ngyn
{
  class Font
  {
    public:
    struct CreateInfo
    {
      std::string path;
      std::string name;
      int size = 16;
      bool pixelated = false;
    };

    struct Character
    {
      float xOffset;
      float yOffset;
      float width;
      float height;
      float bearingX;
      float bearingY;
      float advanceWidth;
    };

    public:
    Font(void) = default;
    Font(CreateInfo createInfo);

    std::weak_ptr<Texture> texture();
    const std::string &name();
    const int &size();
    const bool &pixelated();
    const float &maxHeight();
    const float &spaceAdvance();
    const std::unordered_map<unsigned char, Character> &characters();

    void destroy();
    bool isValid();

    private:
    std::weak_ptr<Texture> _texture;
    std::string _name;
    int _size;
    bool _pixelated;
    float _maxHeight;
    float _spaceAdvance;
    std::unordered_map<unsigned char, Character> _characters;

    std::string getTextureName();
  };
};