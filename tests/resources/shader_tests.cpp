#include <catch2/catch_test_macros.hpp>
#include <ngyn/core/window.hpp>
#include <ngyn/util/files.hpp>
#include <ngyn/resources/shader.hpp>
#include <limits>

using namespace ngyn;

TEST_CASE("Initialization", "[shader]")
{
  Window window(WindowCreateInfo{});
  ngyn::logger.setMode(LoggerMode::Quiet);

  SECTION("Should be valid when creating from vertex and fragment code inline")
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

    Shader shader{{
      .vShaderData = vShaderData,
      .fShaderData = fShaderData
    }};

    REQUIRE(shader.isValid());
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

    Shader shader{{
      .vShaderPath = "shader.vert",
      .fShaderPath = "shader.frag",
    }};

    REQUIRE(shader.isValid());
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

    Shader shader{{
      .vShaderData = vShaderData,
      .fShaderPath = "shader.frag",
    }};

    REQUIRE(shader.isValid());
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

    Shader shader{{
      .fShaderData = fShaderData,
      .vShaderPath = "shader.vert",
    }};

    REQUIRE(shader.isValid());
  }

  SECTION("Should be invalid if shader is destroyed")
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

    Shader shader{{
      .vShaderData = vShaderData,
      .fShaderData = fShaderData,
    }};

    shader.destroy();

    REQUIRE(!shader.isValid());
  }
}

TEST_CASE("setUniform", "[shader]")
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

  Shader shader{{ .vShaderData = vShaderData, .fShaderData = fShaderData }};
  shader.use();

  SECTION("Location should be stored after setting a new uniform")
  {
    shader.setUniform("testTexture", 1);
    auto locations = shader.locations();

    REQUIRE(locations.find("testTexture") != locations.end());
  }
}