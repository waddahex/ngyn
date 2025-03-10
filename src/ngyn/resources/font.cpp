#include "font.hpp"
#include "resources_manager.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H

using namespace ngyn;

Font::Font(CreateInfo createInfo) :
  _name(createInfo.name),
  _size(createInfo.size),
  _pixelated(createInfo.pixelated),
  _maxHeight(0.0f),
  _spaceAdvance(0.0f)
{
  if(_name.empty()) _name = createInfo.path;

  FT_Library ft;
  auto ftError = FT_Init_FreeType(&ft);

  ASSERT(!ftError, "Could not initialize FreeType");

  FT_Face face;
  auto faceError = FT_New_Face(ft, createInfo.path.c_str(), 0, &face);

  ASSERT(!faceError, "Could not create font face {}", createInfo.path);

  FT_Set_Pixel_Sizes(face, 0, createInfo.size);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  int atlasWidth = 0;
  int atlasHeight = 0;
  int padding = 2;

  for(unsigned char c = 32; c < 128; c++)
  {
    if(FT_Load_Char(face, c, FT_LOAD_RENDER))
    {
      LOGGER_WARNING("Could not load char {} from font {}", c, createInfo.path);
      continue;
    }

    atlasWidth += face->glyph->bitmap.width + padding;
    atlasHeight = std::max(atlasHeight, static_cast<int>(face->glyph->bitmap.rows));

    // TODO: Find a way to get a more apropriate space advance
    if(c == ' ')
    {
      _spaceAdvance = face->glyph->metrics.horiAdvance / 64.0f;
    }
  }

  // Height of the bigger char
  _maxHeight = atlasHeight;

  unsigned char *atlasBuffer = new unsigned char[atlasWidth * atlasHeight]();
  int xOffset = 0;

  for(unsigned char c = 32; c < 128; c++)
  {
    if(FT_Load_Char(face, c, FT_LOAD_RENDER))
    {
      LOGGER_WARNING("Could not load char {} from font {}", c, createInfo.path);
      continue;
    }

    auto glyph = face->glyph;

    for(unsigned int row = 0; row < glyph->bitmap.rows; row++)
    {
      for(unsigned int col = 0; col < glyph->bitmap.width; col++)
      {
        atlasBuffer[(row * atlasWidth) + xOffset + col] = 
          glyph->bitmap.buffer[row * glyph->bitmap.width + col];
      }
    }

    _characters[c] = {
      static_cast<float>(xOffset),
      0.0f,
      static_cast<float>(glyph->bitmap.width),
      static_cast<float>(glyph->bitmap.rows),
      static_cast<float>(glyph->bitmap_left),
      static_cast<float>(glyph->bitmap_top),
      static_cast<float>(glyph->advance.x) / 64.0f,
    };

    xOffset += glyph->bitmap.width + padding;
  }

  _texture = ResourcesManager::addResource<Texture>(
    getTextureName(),
    Texture({
      .filtering = createInfo.pixelated ? Texture::Filtering::Nearest : Texture::Filtering::Linear,
      .size = glm::ivec2(atlasWidth, atlasHeight),
      .data = atlasBuffer,
    })
  );

  delete[] atlasBuffer;
  FT_Done_Face(face);
  FT_Done_FreeType(ft);
}

std::weak_ptr<Texture> Font::texture()
{
  return _texture;
}

const std::string &Font::name()
{
  return _name;
}

const int &Font::size()
{
  return _size;
}

const bool &Font::pixelated()
{
  return _pixelated;
}

const float &Font::maxHeight()
{
  return _maxHeight;
}

const float &Font::spaceAdvance()
{
  return _spaceAdvance;
}

const std::unordered_map<unsigned char, Font::Character> &Font::characters()
{
  return _characters;
}

Font::Character Font::getChar(unsigned char c)
{
  ASSERT(
    _characters.find(c) != _characters.end(),
    "Char {} doesn't exist on font {}. TODO: Implement default char",
    c,
    _name
  );

  return _characters[c];
}

void Font::destroy()
{
  if(!_texture.lock()) return;

  auto texture = _texture.lock().get();
  texture->destroy();

  ResourcesManager::removeResource<Texture>(getTextureName());
}

bool Font::isValid()
{
  return !!_texture.lock();
}

std::string Font::getTextureName()
{
  return std::format("{}_{}", _name, _size);
}