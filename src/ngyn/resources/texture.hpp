#pragma once

#include "../../ngynpch.hpp"

#include "../util/logger.hpp"

namespace ngyn
{
  enum class TextureFiltering
  {
    Linear,
    Nearest
  };

  struct TextureCreateInfo
  {
    std::string image;
    TextureFiltering filtering = TextureFiltering::Linear;
    // For font textures
    int width;
    int height;
    unsigned char *data = nullptr;
  };

  class Texture
  {
    public:
    Texture(void) = default;
    Texture(TextureCreateInfo createInfo);

    int index;
    GLuint handle;
    int width;
    int height;

    void bind();
    void destroy();

    private:
    static int indexCount;
    static std::vector<int> unusedIndexes;

    void setIndex();
  };
};