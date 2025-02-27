#pragma once

#include <iostream>
#include <glad/glad.h>

namespace ngyn
{
  struct ShaderCreateInfo
  {
    const char* vShaderData;
    const char* fShaderData;
  };

  class Shader
  {
    public:
    Shader(void) = default;
    Shader(ShaderCreateInfo createInfo);

    GLuint handle;

    void validate(GLuint handle, const std::string &type);
    void use();
  };
};