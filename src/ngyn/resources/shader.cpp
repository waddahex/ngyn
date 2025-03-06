#include "shader.hpp"

using namespace ngyn;

Shader::Shader(ShaderCreateInfo createInfo)
{
  this->handle = std::numeric_limits<GLuint>::max();

  auto vShaderFileData = files::read(createInfo.vShaderPath);
  auto fShaderFileData = files::read(createInfo.fShaderPath);

  if(!vShaderFileData.empty())
  {
    createInfo.vShaderData = vShaderFileData;
  }

  if(!fShaderFileData.empty())
  {
    createInfo.fShaderData = fShaderFileData;
  }

  if(createInfo.vShaderData.empty() || createInfo.fShaderData.empty())
  {
    return;
  }

  const char *vShaderData = createInfo.vShaderData.c_str();
  const char *fShaderData = createInfo.fShaderData.c_str();

  GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vShader, 1, &vShaderData, nullptr);
  glCompileShader(vShader);
  auto vertexValidationResult = this->validate(vShader, "VERTEX_SHADER");

  if(vertexValidationResult != ShaderValidationResult::Valid)
  {
    glDeleteShader(vShader);
    return;
  }

  GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fShader, 1, &fShaderData, nullptr);
  glCompileShader(fShader);
  auto fragmentValidationResult = this->validate(fShader, "FRAGMENT_SHADER");

  if(fragmentValidationResult != ShaderValidationResult::Valid)
  {
    glDeleteShader(vShader);
    glDeleteShader(fShader);
    return;
  }

  this->handle = glCreateProgram();
  glAttachShader(this->handle, vShader);
  glAttachShader(this->handle, fShader);
  glLinkProgram(this->handle);

  auto programValidationResult = this->validate(this->handle, "SHADER_PROGRAM");
  if(programValidationResult != ShaderValidationResult::Valid)
  {
    glDeleteProgram(this->handle);
    this->handle = std::numeric_limits<GLuint>::max();
  }

  glDeleteShader(vShader);
  glDeleteShader(fShader);
}

ShaderValidationResult Shader::validate(GLuint handle, const std::string &type)
{
  int success;
  char infoLog[1024];

  if(type != "SHADER_PROGRAM")
  {
    glGetShaderiv(handle, GL_COMPILE_STATUS, &success);

    if(!success)
    {
      glGetShaderInfoLog(handle, sizeof(infoLog), nullptr, infoLog);

      LOGGER_ERROR("Error compiling {}\n{}", type, infoLog);
      
      return type == "VERTEX_SHADER" ?
        ShaderValidationResult::VertexCompileError :
        ShaderValidationResult::FragmentCompileError;
    }

    return ShaderValidationResult::Valid;
  }

  glGetProgramiv(handle, GL_LINK_STATUS, &success);
  if(!success)
  {
    glGetShaderInfoLog(handle, sizeof(infoLog), nullptr, infoLog);
    LOGGER_ERROR("Error linking {}\n{}", type, infoLog);

    return ShaderValidationResult::ProgramLinkError;
  }

  return ShaderValidationResult::Valid;
}

GLint ngyn::Shader::getLocation(const std::string &location)
{
  if(this->locations.find(location) == this->locations.end())
  {
    this->locations[location] = glGetUniformLocation(this->handle, location.c_str());
  }

  return this->locations[location];
}

void ngyn::Shader::setInt(const std::string &location, int value)
{
  glUniform1i(this->getLocation(location), value);
}

void ngyn::Shader::setVec4(const std::string &location, const glm::vec4 &value)
{
  glUniform4fv(this->getLocation(location), 1, &value[0]);
}

void ngyn::Shader::setMat4(const std::string &location, const glm::mat4 &value)
{
  glUniformMatrix4fv(this->getLocation(location), 1, GL_FALSE, &value[0][0]);
}

void ngyn::Shader::use()
{
  glUseProgram(this->handle);
}

void ngyn::Shader::destroy()
{
  glDeleteProgram(this->handle);
  this->handle = std::numeric_limits<GLuint>::max();
}