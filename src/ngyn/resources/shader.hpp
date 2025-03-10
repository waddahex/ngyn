#pragma once

#include "../../ngynpch.hpp"

#include "../util/logger.hpp"
#include "../util/files.hpp"

namespace ngyn
{
  class Shader
  {
    public:
    struct CreateInfo
    {
      // For creating shader from inline code
      std::string vShaderData;            // Vertex shader code
      std::string fShaderData;            // Fragment shader code
      // For createing shader from file
      std::filesystem::path vShaderPath;  // Vetex shader file
      std::filesystem::path fShaderPath;  // Fragment shader file
    };

    public:
    Shader(void) = default;
    Shader(CreateInfo createInfo);

    const GLuint &handle();
    const std::unordered_map<std::string, GLint> &locations();

    template<typename T>
    void setUniform(const std::string &location, T value)
    {
      ASSERT(isValid(), "Tried to set uniform using an invalid shader");

      use();

      if(std::is_same_v<int, T>)
      {
        glUniform1i(this->getLocation(location), value);
        return;
      }

      if(std::is_same_v<glm::vec4, T>)
      {
        glm::vec4 vec4 = static_cast<glm::vec4>(value);
        glUniform4fv(this->getLocation(location), 1, &vec4[0]);
        return;
      }

      if(std::is_same_v<glm::mat4, T>)
      {
        glm::mat4 mat4 = static_cast<glm::mat4>(value);
        glUniformMatrix4fv(this->getLocation(location), 1, GL_FALSE, &mat4[0][0]);
        return;
      }

      ASSERT(false, "No setUniform operation for the type {}", typeid(T).name());
    }

    void use();
    void destroy();
    bool isValid();
    
    private:
    GLuint _handle;
    std::unordered_map<std::string, GLint> _locations;

    void validate(GLuint handle, const std::string &type);
    GLint getLocation(const std::string &location);
  };
};