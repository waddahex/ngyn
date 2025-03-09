#include "color.hpp"

using namespace ngyn;

Color::Color(int intR, int intG, int intB, int intA)
{
  r = intR / 255.0f;
  g = intG / 255.0f;
  b = intB / 255.0f;
  a = intA / 255.0f;
}

Color::Color(int intR, int intG, int intB)
{
  r = intR / 255.0f;
  g = intG / 255.0f;
  b = intB / 255.0f;
  w = 1.0f;
}

Color::Color(int rgba)
{
  r = rgba / 255.0f;
  g = rgba / 255.0f;
  b = rgba / 255.0f;
  a = rgba / 255.0f;
}

Color::Color(const std::string &hexColor)
{
  int intR = 255;
  int intG = 255;
  int intB = 255;
  int intA = 255;

  std::string hexCode = hexColor;

  // #RGB -> #RRGGBB
  if(hexCode[0] == '#' && hexCode.length() == 4)
  {
    hexCode = std::format("#{0}{0}{1}{1}{2}{2}", hexCode[1], hexCode[2], hexCode[3]);
  }

  // #RGBA -> #RRGGBBAA
  if(hexCode[0] == '#' && hexCode.length() == 5)
  {
    hexCode = std::format("#{0}{0}{1}{1}{2}{2}{3}F", hexCode[1], hexCode[2], hexCode[3], hexCode[4]);
  }

  // #RRGGBB
  if(hexCode[0] == '#' && hexCode.length() == 7)
  {
    sscanf_s(hexCode.c_str(), "#%02x%02x%02x", &intR, &intG, &intB);
  }

  // #RRGGBBAA
  if(hexCode[0] == '#' && hexCode.length() == 9)
  {
    sscanf_s(hexCode.c_str(), "#%02x%02x%02x%02x", &intR, &intG, &intB, &intA);
  }

  r = intR / 255.0f;
  g = intG / 255.0f;
  b = intB / 255.0f;
  a = intA / 255.0f;
}