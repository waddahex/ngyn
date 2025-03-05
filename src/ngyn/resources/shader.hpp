#pragma once

#include <iostream>
#include <glad/glad.h>
#include <filesystem>
#include <glm/glm.hpp>

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
    ~Shader();

    GLuint handle;

    void setInt(const std::string &location, int value);
    void setVec4(const std::string &location, const glm::vec4 &value);
    void setMat4(const std::string &location, const glm::mat4 &value);
    void use();
    void destroy();

    private:
    std::unordered_map<std::string, GLint> locations;
    ShaderValidationResult validate(GLuint handle, const std::string &type);
    GLint getLocation(const std::string &location);
  };
};