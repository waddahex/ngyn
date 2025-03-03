#pragma once

#include <iostream>
#include <glad/glad.h>
#include <filesystem>

#include "../util/logger.hpp"
#include "../util/files.hpp"

namespace ngyn
{
  struct ShaderCreateInfo
  {
    std::string vShaderData;            // Vertex shader code
    std::string fShaderData;            // Fragment shader code
    std::filesystem::path vShaderPath;  // Vetex shader file
    std::filesystem::path fShaderPath;  // Fragment shader file
  };

  enum ShaderValidationResult
  {
    Valid = 0,
    VertexCompileError = 1,
    FragmentCompileError = 2,
    ProgramLinkError = 3
  };

  class Shader
  {
    public:
    Shader(void) = default;
    Shader(ShaderCreateInfo createInfo);

    GLuint handle;

    ShaderValidationResult validate(GLuint handle, const std::string &type);
    void use();
  };
};