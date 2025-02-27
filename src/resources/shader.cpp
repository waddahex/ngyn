#include "shader.hpp"

ddengine::Shader::Shader(ShaderCreateInfo createInfo)
{
  GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vShader, 1, &createInfo.vShaderData, nullptr);
  glCompileShader(vShader);
  this->validate(vShader, "VERTEX_SHADER");

  GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fShader, 1, &createInfo.fShaderData, nullptr);
  glCompileShader(fShader);
  this->validate(fShader, "FRAGMENT_SHADER");

  this->handle = glCreateProgram();
  glAttachShader(this->handle, vShader);
  glAttachShader(this->handle, fShader);
  glLinkProgram(this->handle);

  this->validate(vShader, "SHADER_PROGRAM");
}

void ddengine::Shader::validate(GLuint handle, const std::string &type)
{
  int success;
  char infoLog[1024];

  if(type != "SHADER_PROGRAM")
  {
    glGetShaderiv(handle, GL_COMPILE_STATUS, &success);

    if(!success)
    {
      glGetShaderInfoLog(handle, sizeof(infoLog), nullptr, infoLog);
      std::cout << "Error compiling " << type << '\n' << infoLog << std::endl;
    }

    return;
  }

  glGetProgramiv(handle, GL_LINK_STATUS, &success);
  if(!success)
  {
    glGetShaderInfoLog(handle, sizeof(infoLog), nullptr, infoLog);
    std::cout << "Error linking " << type << '\n' << infoLog << std::endl;
  }
}

void ddengine::Shader::use()
{
  glUseProgram(this->handle);
}
