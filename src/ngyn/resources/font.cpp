#include "font.hpp"
#include "resources_manager.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H

using namespace ngyn;

Font::Font(FontCreateInfo createInfo)
{
  FT_Library ft;
  FT_Init_FreeType(&ft);

  FT_Face face;
  FT_New_Face(ft, createInfo.path.c_str(), 0, &face);

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

    // Use o's width as space width
    if(c == 'o')
    {
      this->spaceAdvance = face->glyph->bitmap.width;
    }
  }

  // Height of the bigger char
  this->maxHeight = atlasHeight;

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

    this->characters[c] = {
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

  this->texture = ResourcesManager::addResource<Texture>(
    createInfo.name + "_" + std::to_string(createInfo.size),
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

void ngyn::Font::destroy()
{
  if(this->texture.expired()) return;
  
  this->texture.lock().get()->destroy();
}
