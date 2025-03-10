#include "texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

using namespace ngyn;

const int MAX_TEXTURES = 32;
int Texture::_indexCount = 0;
std::vector<int> Texture::_unusedIndexes;

Texture::Texture(CreateInfo createInfo) :
  _index(-1),
  _handle(std::numeric_limits<GLuint>::max()),
  _size(createInfo.size)
{
  int channels = 1;

  bool wasLoadedFromStb = false;

  // If doesn't have data and have image, should be loaded from file and file should exist
  if(!createInfo.data && !createInfo.image.empty())
  {
    ASSERT(std::filesystem::exists(createInfo.image), "Could not find image {}", createInfo.image);
    createInfo.data = stbi_load(createInfo.image.c_str(), &_size.x, &_size.y, &channels, 0);
    wasLoadedFromStb = true;
  }

  // If still doesn't have data something is wrong with the image file provided
  ASSERT(createInfo.data, "Could no load data from image {}", createInfo.image);

  setIndex();

  glGenTextures(1, &_handle);
  glBindTexture(GL_TEXTURE_2D, _handle);

  int format = GL_RGBA; // transparency

  if(channels == 1) format = GL_RED; // Used for text, only one color
  else if(channels == 3) format = GL_RGB; // no transparency

  glTexImage2D(
    GL_TEXTURE_2D,
    0,
    format,
    _size.x,
    _size.y,
    0,
    format,
    GL_UNSIGNED_BYTE,
    createInfo.data
  );

  glGenerateMipmap(GL_TEXTURE_2D);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(
    GL_TEXTURE_2D,
    GL_TEXTURE_MIN_FILTER,
    createInfo.filtering == Filtering::Linear ? GL_LINEAR : GL_NEAREST
  );

  glTexParameteri(
    GL_TEXTURE_2D,
    GL_TEXTURE_MAG_FILTER,
    createInfo.filtering == Filtering::Linear ? GL_LINEAR : GL_NEAREST
  );

  if(wasLoadedFromStb) stbi_image_free(createInfo.data);

  glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::bind()
{
  ASSERT(isValid(), "Tried to bind an invalid texture");

  glActiveTexture(GL_TEXTURE0 + _index);
  glBindTexture(GL_TEXTURE_2D, _handle);
}

void Texture::destroy()
{
  if(_handle != std::numeric_limits<GLuint>::max())
  {
    glDeleteTextures(1, &_handle);
  }
  
  if(_index != -1)
  {
    _unusedIndexes.push_back(_index);
  }
  
  _handle = std::numeric_limits<GLuint>::max();
  _index = -1;
}

bool Texture::isValid()
{
  return _index > -1 && _handle != std::numeric_limits<GLuint>::max();
}

const glm::ivec2 &Texture::size()
{
  return _size;
}

void Texture::setIndex()
{
  // Gets the index from the unused index if there is any
  if(!_unusedIndexes.empty())
  {
    _index = _unusedIndexes[0];
    _unusedIndexes.erase(_unusedIndexes.begin());

    return;
  }

  ASSERT(_indexCount <= MAX_TEXTURES, "Reached max amount of textures {}", MAX_TEXTURES);

  _index = _indexCount;
  _indexCount++;
}

const int &Texture::index()
{
  return _index;
}

const GLuint &Texture::handle()
{
  return _handle;
}