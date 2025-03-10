#pragma once

#include "../../ngynpch.hpp"

#include "../util/logger.hpp"

namespace ngyn
{
  class Texture
  {
    public:
    enum Filtering
    {
      Linear,
      Nearest
    };

    struct CreateInfo
    {
      std::string image;
      Filtering filtering = Filtering::Linear;
      glm::ivec2 size;
      unsigned char *data = nullptr;
    };

    public:
    Texture(void) = default;
    Texture(CreateInfo createInfo);

    void bind();
    void destroy();
    bool isValid();

    const int &index();
    const GLuint &handle();
    const glm::ivec2 &size();

    private:
    static int _indexCount;
    static std::vector<int> _unusedIndexes;

    int _index;
    GLuint _handle;
    glm::ivec2 _size;

    void setIndex();
  };
};