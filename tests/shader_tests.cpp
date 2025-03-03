#include <catch2/catch_test_macros.hpp>
#include <core/window.hpp>
#include <util/files.hpp>
#include <resources/shader.hpp>
#include <limits>

using namespace ngyn;

TEST_CASE("Shader validation", "[shader]")
{
  Window window(WindowCreateInfo{});
  ngyn::logger.setMode(LoggerMode::Quiet);

  SECTION("Handle should be different than GLuint max value")
  {
    const char *vShaderData = R"(
    #version 330 core
    layout (location = 0) in vec2 aPos;
    void main()
    {
      gl_Position = vec4(aPos, 0.0, 1.0);
    }
    )";

    const char *fShaderData = R"(
    #version 330 core
    out vec4 FragColor;
    void main()
    {
      FragColor = vec4(1.0);
    }
    )";

    Shader shader(ShaderCreateInfo{
      .vShaderData = vShaderData,
      .fShaderData = fShaderData
    });

    REQUIRE(shader.handle != std::numeric_limits<GLuint>::max());
  }

  SECTION("Handle should be equal GLuint max value if vertex shader is invalid")
  {
    const char *vShaderData = R"(
    #version 330 core
    layout (location = 0) in vec2 aPos;

    void main()
    {
      position = vec4(aPos, 0.0, 1.0);
    }
    )";

    const char *fShaderData = R"(
    #version 330 core
    out vec4 FragColor;
    void main()
    {
      FragColor = vec4(1.0);
    }
    )";

    Shader shader(ShaderCreateInfo{
      .vShaderData = vShaderData,
      .fShaderData = fShaderData
    });

    REQUIRE(shader.handle == std::numeric_limits<GLuint>::max());
  }

  SECTION("Handle should be equal GLuint max value if fragment shader is invalid")
  {
    const char *vShaderData = R"(
    #version 330 core
    layout (location = 0) in vec2 aPos;

    void main()
    {
      gl_Position = vec4(aPos, 0.0, 1.0);
    }
    )";

    const char *fShaderData = R"(
    #version 330 core
    out vec4 FragColor;
    void main()
    {
      FragmentColor = vec4(1.0);
    }
    )";

    Shader shader(ShaderCreateInfo{
      .vShaderData = vShaderData,
      .fShaderData = fShaderData
    });

    REQUIRE(shader.handle == std::numeric_limits<GLuint>::max());
  }

  SECTION("Should load shaders from file")
  {
    const char *vShaderData = R"(
    #version 330 core
    layout (location = 0) in vec2 aPos;
    void main()
    {
      gl_Position = vec4(aPos, 0.0, 1.0);
    }
    )";

    const char *fShaderData = R"(
    #version 330 core
    out vec4 FragColor;
    void main()
    {
      FragColor = vec4(1.0);
    }
    )";

    files::write("shader.vert", vShaderData, {.recursive = true});
    files::write("shader.frag", fShaderData, {.recursive = true});

    Shader shader(ShaderCreateInfo{
      .vShaderPath = "shader.vert",
      .fShaderPath = "shader.frag",
    });

    REQUIRE(shader.handle != std::numeric_limits<GLuint>::max());
  }

  SECTION("Should use vShaderData if vShaderPath is not provided")
  {
    const char *vShaderData = R"(
    #version 330 core
    layout (location = 0) in vec2 aPos;
    void main()
    {
      gl_Position = vec4(aPos, 0.0, 1.0);
    }
    )";

    const char *fShaderData = R"(
    #version 330 core
    out vec4 FragColor;
    void main()
    {
      FragColor = vec4(1.0);
    }
    )";

    files::write("shader.frag", fShaderData, {.recursive = true});

    Shader shader(ShaderCreateInfo{
      .vShaderData = vShaderData,
      .fShaderPath = "shader.frag",
    });

    REQUIRE(shader.handle != std::numeric_limits<GLuint>::max());
  }

  SECTION("Should use fShaderData if fShaderPath is not provided")
  {
    const char *vShaderData = R"(
    #version 330 core
    layout (location = 0) in vec2 aPos;
    void main()
    {
      gl_Position = vec4(aPos, 0.0, 1.0);
    }
    )";

    const char *fShaderData = R"(
    #version 330 core
    out vec4 FragColor;
    void main()
    {
      FragColor = vec4(1.0);
    }
    )";

    files::write("shader.vert", vShaderData, {.recursive = true});

    Shader shader(ShaderCreateInfo{
      .fShaderData = fShaderData,
      .vShaderPath = "shader.vert",
    });

    REQUIRE(shader.handle != std::numeric_limits<GLuint>::max());
  }

  SECTION("Handle should be equal GLuint max value if both path and data is not provided")
  {
    Shader shader(ShaderCreateInfo{});
    REQUIRE(shader.handle == std::numeric_limits<GLuint>::max());
  }
}