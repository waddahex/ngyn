#include "texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

using namespace ngyn;

const int MAX_TEXTURES = 32;
int Texture::indexCount = 0;
std::vector<int> Texture::unusedIndexes;

Texture::Texture(TextureCreateInfo createInfo)
{
  this->handle = std::numeric_limits<GLuint>::max();
  this->index = std::numeric_limits<GLuint>::max();
  this->width = createInfo.width;
  this->height = createInfo.height;

  if(!createInfo.data && !std::filesystem::exists(createInfo.image))
  {
    LOGGER_ERROR("Could not find image {}", createInfo.image);
    return;
  }

  int channels = 1;

  if(!createInfo.data)
  {
    createInfo.data = stbi_load(createInfo.image.c_str(), &this->width, &this->height, &channels, 0);
  }

  if(!createInfo.data)
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
    createInfo.data
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

  if(!createInfo.image.empty())
  {
    stbi_image_free(createInfo.data);
  }

  glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::bind()
{
  glActiveTexture(GL_TEXTURE0 + this->index);
  glBindTexture(GL_TEXTURE_2D, this->handle);
}

void Texture::destroy()
{
  glDeleteTextures(1, &this->handle);
  this->handle = std::numeric_limits<GLuint>::max();

  if(this->index != std::numeric_limits<GLuint>::max())
  {
    this->unusedIndexes.push_back(this->index);
  }
}

void Texture::setIndex()
{
  if(!this->unusedIndexes.empty())
  {
    this->index = this->unusedIndexes[0];
    this->unusedIndexes.erase(this->unusedIndexes.begin());

    return;
  }

  if(this->indexCount > MAX_TEXTURES)
  {
    LOGGER_ERROR("Reached max amount of textures {}", MAX_TEXTURES);

    return;
  }

  this->index = this->indexCount;
  this->indexCount++;
}