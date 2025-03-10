#include "quad_renderer.hpp"

ngyn::QuadRenderer::QuadRenderer()
{
  this->size = 2;
  this->stride = sizeof(float) * 2;
  this->drawMode = GL_TRIANGLES;
  this->shaderName = "quad_renderer_shader";

  this->vertices = {
    0.0f, 0.0f, // bottom-left
    0.0f, 1.0f, // top-left
    1.0f, 1.0f, // top-right
    1.0f, 0.0f, // bottom-right
  };

  this->indices = {
    0, 1, 2,
    0, 2, 3
  };
}

void ngyn::QuadRenderer::onSetup()
{
  const char *vShaderData =
R"(
  #version 460 core
  layout (location = 0) in vec2 aPos;

  struct InstanceData
  {
    mat4 mvp;
    vec4 texCoords1;
    vec4 texCoords2;
    vec4 color;
    int textureID;
    int zIndex;
    int padding[2];
  };

  out VS_OUT
  {
    vec2 texCoords;
    vec4 color;
    flat int textureID;
  } vs_out;

  layout (std430, binding = 0) buffer InstanceBuffer {
    InstanceData instancesData[];
  };

  void main()
  {
    InstanceData instanceData = instancesData[gl_InstanceID];

    gl_Position = instanceData.mvp * vec4(aPos, 0.0, 1.0);

    if(gl_VertexID == 0) vs_out.texCoords = instanceData.texCoords1.xy;
    else if(gl_VertexID == 1) vs_out.texCoords = instanceData.texCoords1.zw;
    else if(gl_VertexID == 2) vs_out.texCoords = instanceData.texCoords2.xy;
    else vs_out.texCoords = instanceData.texCoords2.zw;

    vs_out.color = instanceData.color;
    vs_out.textureID = instanceData.textureID;
  }
)";

  const char *fShaderData =
R"(
  #version 460 core
  out vec4 FragColor;

  in VS_OUT
  {
    vec2 texCoords;
    vec4 color;
    flat int textureID;
  } fs_in;

  uniform sampler2D textures[32];

  void main()
  {
    if(fs_in.textureID == -1)
    {
      FragColor = fs_in.color;
      return;
    }

    vec4 sampled = texture(textures[fs_in.textureID], fs_in.texCoords);
    FragColor = sampled * vec4(1.0, 1.0, 1.0, fs_in.color.a);
  }
)";

  this->shader = ResourcesManager::addResource<Shader>(this->shaderName, Shader(
    {.vShaderData = vShaderData, .fShaderData = fShaderData}
  ));
}

void ngyn::QuadRenderer::onRender()
{
  ASSERT(this->shader.lock(), "Shader not present on QuadRenderer");

  auto shaderPtr = this->shader.lock().get();
  auto textureStorage = ResourcesManager::getStorage<Texture>();

  shaderPtr->use();

  for(auto &[_, texture] : textureStorage.resources)
  {
    auto texturePtr = texture.get();

    shaderPtr->setInt(std::format("textures[{}]", texturePtr->index()), texturePtr->index());
    texturePtr->bind();
  }

  this->orderedData = this->instancesData;

  std::sort(this->orderedData.begin(), this->orderedData.end(),
    [](const QuadInstanceData &lhs, const QuadInstanceData &rhs)
    {
      return lhs.zIndex < rhs.zIndex;
    }
  );
}