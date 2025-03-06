#include <catch2/catch_test_macros.hpp>
#include <ngyn/core/window.hpp>
#include <ngyn/renderers/quad_renderer.hpp>
#include <ngyn/util/logger.hpp>

using namespace ngyn;

static Window window(WindowCreateInfo{});

class TestQuadRenderer : public QuadRenderer
{
  public:
  TestQuadRenderer() : QuadRenderer(){};
  
  auto getOrderedInstances() { return this->orderedData; }
  auto getInstancesData() { return this->instancesData; }
  auto getUnusedInstances() { return this->unusedInstances; }
  auto getShader() { return this->shader; }
};

TEST_CASE("Initialization", "[quad_renderer]")
{
  ngyn::logger.setLevel(LoggerLevel::Disabled);

  SECTION("Shader should be valid")
  {
    TestQuadRenderer tqr;
    tqr.setup();

    auto shader = tqr.getShader();

    REQUIRE(shader->handle != std::numeric_limits<GLuint>::max());
  }
}

TEST_CASE("Instances", "[quad_renderer]")
{
  ngyn::logger.setLevel(LoggerLevel::Disabled);

  SECTION("Returned index should be equal of size -1")
  {
    TestQuadRenderer tqr;

    int index = tqr.addInstance(QuadInstanceData{});
    auto instancesData = tqr.getInstancesData();

    REQUIRE(index == instancesData.size() -1);
  }

  SECTION("Unused instances should contain the removed instance index")
  {
    TestQuadRenderer tqr;

    int index = tqr.addInstance(QuadInstanceData{});

    tqr.removeInstance(index);
    auto unusedInstances = tqr.getUnusedInstances();

    REQUIRE(index == unusedInstances[0]);
  }

  SECTION("Should get the next index from unusedInstances")
  {
    TestQuadRenderer tqr;

    int index = tqr.addInstance(QuadInstanceData{});

    tqr.removeInstance(index);

    int newIndex = tqr.addInstance(QuadInstanceData{});

    REQUIRE(index == newIndex);
  }

  SECTION("Should remove index from unusedInstances after getting it")
  {
    TestQuadRenderer tqr;

    int index = tqr.addInstance(QuadInstanceData{});

    tqr.removeInstance(index);

    int newIndex = tqr.addInstance(QuadInstanceData{});

    auto unusedInstances = tqr.getUnusedInstances();

    REQUIRE(unusedInstances.empty());
  }
}

TEST_CASE("Render", "[quad_renderer]")
{
  ngyn::logger.setLevel(LoggerLevel::Disabled);

  SECTION("Orderer instances should not be empty")
  {
    TestQuadRenderer tqr;

    tqr.setup();
    tqr.addInstance(QuadInstanceData{.zIndex = 0.1f});
    tqr.onRender();

    auto orderedInstances = tqr.getOrderedInstances();

    REQUIRE(!orderedInstances.empty());
  }

  SECTION("Orderer instances should be ordered based on zIndex")
  {
    TestQuadRenderer tqr;

    tqr.setup();

    tqr.addInstance(QuadInstanceData{.zIndex = 0.3f});
    tqr.addInstance(QuadInstanceData{.zIndex = 0.2f});
    tqr.addInstance(QuadInstanceData{.zIndex = 0.1f});

    tqr.onRender();

    auto orderedInstances = tqr.getOrderedInstances();

    REQUIRE(orderedInstances[0].zIndex == 0.1f);
    REQUIRE(orderedInstances[2].zIndex == 0.3f);
  }
}