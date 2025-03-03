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

  auto programValidationResult = this->validate(vShader, "SHADER_PROGRAM");
  if(programValidationResult != ShaderValidationResult::Valid)
  {
    glDeleteProgram(this->handle);
    this->handle = std::numeric_limits<GLuint>::max();
  }

  glDeleteShader(vShader);
  glDeleteShader(fShader);
}

ngyn::Shader::~Shader()
{
  this->destroy();
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

void ngyn::Shader::use()
{
  glUseProgram(this->handle);
}

void ngyn::Shader::destroy()
{
  glDeleteProgram(this->handle);
  this->handle = std::numeric_limits<GLuint>::max();
}