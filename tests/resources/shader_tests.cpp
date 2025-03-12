#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <ngyn/ngyn.hpp>

using namespace ngyn;

TEST_CASE("Initialization")
{
  Window window{{}};
  ngyn::logger.setMode(LoggerMode::Quiet);

  SUBCASE("Should be valid when creating from vertex and fragment code inline")
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

    CHECK(shader.isValid());
  }

  SUBCASE("Should load shaders from file")
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

    CHECK(shader.isValid());
  }

  SUBCASE("Should use vShaderData if vShaderPath is not provided")
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

    CHECK(shader.isValid());
  }

  SUBCASE("Should use fShaderData if fShaderPath is not provided")
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

    CHECK(shader.isValid());
  }

  SUBCASE("Should be invalid if shader is destroyed")
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

    CHECK(!shader.isValid());
  }
}

TEST_CASE("setUniform")
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

  SUBCASE("Location should be stored after setting a new uniform")
  {
    shader.setUniform("testTexture", 1);
    auto locations = shader.locations();

    CHECK(locations.find("testTexture") != locations.end());
  }
}