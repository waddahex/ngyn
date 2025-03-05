#include "texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

using namespace ngyn;

const int MAX_TEXTURES = 32;
GLuint Texture::indexCount = 0;
std::vector<GLuint> Texture::unusedIndexes;

Texture::Texture(TextureCreateInfo createInfo)
{
  this->handle = std::numeric_limits<GLuint>::max();
  this->index = std::numeric_limits<GLuint>::max();

  if(!createInfo.data && !std::filesystem::exists(createInfo.image))
  {
    LOGGER_ERROR("Could not find image {}", createInfo.image);
    return;
  }

  int channels;
  unsigned char *data = stbi_load(createInfo.image.c_str(), &this->width, &this->height, &channels, 0);

  if(!data)
  {
    LOGGER_ERROR("Could no load data from image {}", createInfo.image);
    return;
  }

  this->setIndex();

  if(this->index == std::numeric_limits<GLuint>::max())
  {
    return;
  }

  glGenTextures(1, &this->handle);
  glBindTexture(GL_TEXTURE_2D, this->handle);

  int format = GL_RGBA;

  if(channels == 1) format = GL_RED;
  else if(channels == 3) format = GL_RGB;

  glTexImage2D(
    GL_TEXTURE_2D,
    0,
    format,
    this->width,
    this->height,
    0,
    format,
    GL_UNSIGNED_BYTE,
    data
  );

  glGenerateMipmap(GL_TEXTURE_2D);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(
    GL_TEXTURE_2D,
    GL_TEXTURE_MIN_FILTER,
    createInfo.filtering == TextureFiltering::Linear ? GL_LINEAR : GL_NEAREST
  );
  glTexParameteri(
    GL_TEXTURE_2D,
    GL_TEXTURE_MAG_FILTER,
    createInfo.filtering == TextureFiltering::Linear ? GL_LINEAR : GL_NEAREST
  );

  stbi_image_free(data);
}

void Texture::bind()
{
  glBindTexture(GL_TEXTURE_2D, this->handle);
  glActiveTexture(GL_TEXTURE0);
}

void Texture::destroy()
{
  glDeleteTextures(1, &this->handle);
  this->handle = std::numeric_limits<GLuint>::max();
  this->unusedIndexes.push_back(this->index);
}

void Texture::setIndex()
{
  if(!this->unusedIndexes.empty())
  {
    this->index = this->unusedIndexes[0];
    this->unusedIndexes.erase(this->unusedIndexes.begin());

    return;
  }

  if(this->indexCount == MAX_TEXTURES)
  {
    LOGGER_ERROR("Reached max amount of textures {}", MAX_TEXTURES);

    return;
  }

  this->indexCount++;
  this->index = this->indexCount;
}