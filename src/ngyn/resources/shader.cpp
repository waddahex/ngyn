#include "shader.hpp"

using namespace ngyn;

Shader::Shader(CreateInfo createInfo) :
  _handle(std::numeric_limits<GLuint>::max())
{
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

  const char *vShaderData = createInfo.vShaderData.c_str();
  const char *fShaderData = createInfo.fShaderData.c_str();

  GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vShader, 1, &vShaderData, nullptr);
  glCompileShader(vShader);
  validate(vShader, "VERTEX_SHADER");

  GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fShader, 1, &fShaderData, nullptr);
  glCompileShader(fShader);
  validate(fShader, "FRAGMENT_SHADER");

  _handle = glCreateProgram();
  glAttachShader(_handle, vShader);
  glAttachShader(_handle, fShader);
  glLinkProgram(_handle);

  validate(_handle, "SHADER_PROGRAM");

  glDeleteShader(vShader);
  glDeleteShader(fShader);
}

void Shader::validate(GLuint handle, const std::string &type)
{
  int success;
  char infoLog[1024];

  // Validate shaders
  if(type != "SHADER_PROGRAM")
  {
    glGetShaderiv(handle, GL_COMPILE_STATUS, &success);

    if(!success)
    {
      glGetShaderInfoLog(handle, sizeof(infoLog), nullptr, infoLog);

      ASSERT(success, "Error compiling {}\n{}", type, infoLog);
    }

    return;
  }

  // Validate shader program
  glGetProgramiv(handle, GL_LINK_STATUS, &success);
  if(!success)
  {
    glGetShaderInfoLog(handle, sizeof(infoLog), nullptr, infoLog);
    ASSERT(success, "Error linking {}\n{}", type, infoLog);
  }
}

GLint Shader::getLocation(const std::string &location)
{
  if(_locations.find(location) == _locations.end())
  {
    _locations[location] = glGetUniformLocation(_handle, location.c_str());
  }

  return _locations[location];
}

const GLuint &Shader::handle()
{
  return _handle;
}

const std::unordered_map<std::string, GLint> &Shader::locations()
{
  return _locations;
}

void Shader::use()
{
  ASSERT(isValid(), "Tried to use an invalid shader program");
  glUseProgram(_handle);
}

void Shader::destroy()
{
  if(!isValid()) return;

  glDeleteProgram(_handle);
  _handle = std::numeric_limits<GLuint>::max();
}

bool Shader::isValid()
{
  return _handle != std::numeric_limits<GLuint>::max();
}