#pragma once

#include "../../ngynpch.hpp"

#include "logger.hpp"

namespace ngyn
{
  class Color : public glm::vec4
  {
    public:
    Color() : glm::vec4(1.0f){};
    Color(int r, int g, int b, int a);
    Color(int r, int g, int b);
    Color(int rgba);

    /**
     * Convert a hex color into rgba and normalize it.  
     *   
     * Supported formats:  
     * #RGB
     * #RGBA
     * #RRGGBB
     * #RRGGBBAA
     * @param hexColor hex color string
     */
    Color(const std::string &hexColor);
  };
};