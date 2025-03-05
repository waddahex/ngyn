#pragma once

#include <filesystem>
#include <glad/glad.h>
#include <ngyn/util/logger.hpp>
#include <numeric>
#include <vector>

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

    GLuint index;
    GLuint handle;
    int width;
    int height;

    void bind();
    void destroy();

    private:
    static GLuint indexCount;
    static std::vector<GLuint> unusedIndexes;

    void setIndex();
  };
};