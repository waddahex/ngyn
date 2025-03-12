#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <ngyn/ngyn.hpp>

/*
  TODO: test getInstance
  TODO: test setInstance
*/

using namespace ngyn;

static Window window{{}};

class TestQuadRenderer : public QuadRenderer
{
  public:
  TestQuadRenderer() : QuadRenderer(){};
  
  auto getOrderedInstances() { return this->orderedData; }
  auto getInstancesData() { return this->instancesData; }
  auto getUnusedInstances() { return this->unusedInstances; }
  auto getShader() { return this->shader; }
};

TEST_CASE("Initialization")
{
  ngyn::logger.setLevel(LoggerLevel::Disabled);

  SUBCASE("Shader should be valid")
  {
    TestQuadRenderer tqr;
    tqr.setup();

    auto shader = tqr.getShader().lock().get();

    CHECK(shader->isValid());
  }
}

TEST_CASE("Instances")
{
  ngyn::logger.setLevel(LoggerLevel::Disabled);

  SUBCASE("Returned index should be equal of size -1")
  {
    TestQuadRenderer tqr;

    int index = tqr.addInstance(QuadInstanceData{});
    auto instancesData = tqr.getInstancesData();

    CHECK(index == instancesData.size() -1);
  }

  SUBCASE("Unused instances should contain the removed instance index")
  {
    TestQuadRenderer tqr;

    int index = tqr.addInstance(QuadInstanceData{});

    tqr.removeInstance(index);
    auto unusedInstances = tqr.getUnusedInstances();

    CHECK(index == unusedInstances[0]);
  }

  SUBCASE("Should get the next index from unusedInstances")
  {
    TestQuadRenderer tqr;

    int index = tqr.addInstance(QuadInstanceData{});

    tqr.removeInstance(index);

    int newIndex = tqr.addInstance(QuadInstanceData{});

    CHECK(index == newIndex);
  }

  SUBCASE("Should remove index from unusedInstances after getting it")
  {
    TestQuadRenderer tqr;

    int index = tqr.addInstance(QuadInstanceData{});

    tqr.removeInstance(index);

    int newIndex = tqr.addInstance(QuadInstanceData{});

    auto unusedInstances = tqr.getUnusedInstances();

    CHECK(unusedInstances.empty());
  }
}

TEST_CASE("Render")
{
  ngyn::logger.setLevel(LoggerLevel::Disabled);

  SUBCASE("Orderer instances should not be empty")
  {
    TestQuadRenderer tqr;

    tqr.setup();
    tqr.addInstance(QuadInstanceData{.zIndex = 0.1f});
    tqr.onRender();

    auto orderedInstances = tqr.getOrderedInstances();

    CHECK(!orderedInstances.empty());
  }

  SUBCASE("Orderer instances should be ordered based on zIndex")
  {
    TestQuadRenderer tqr;

    tqr.setup();

    tqr.addInstance(QuadInstanceData{.zIndex = 0.3f});
    tqr.addInstance(QuadInstanceData{.zIndex = 0.2f});
    tqr.addInstance(QuadInstanceData{.zIndex = 0.1f});

    tqr.onRender();

    auto orderedInstances = tqr.getOrderedInstances();

    CHECK(orderedInstances[0].zIndex == 0.1f);
    CHECK(orderedInstances[2].zIndex == 0.3f);
  }
}