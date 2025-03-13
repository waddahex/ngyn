#define DOCTEST_CONFIG_IMPLEMENT
#include <doctest/doctest.h>

#include <ngyn/ngyn.hpp>

using namespace ngyn;

std::string dataPath = "data";

Window window{{}};

int main(int argc, char **argv)
{
  window.open();
  window.loadGL();

  if(argc > 1)
  {
    dataPath = (std::filesystem::path(argv[1]) / "data").string();
  }

  doctest::Context context;

  context.applyCommandLine(argc, argv);

  int res = context.run();

  if(context.shouldExit()) return res;

  return res;
}

/*
  TODO: test getInstance
  TODO: test setInstance
*/

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
  ngLogger.setLevel(LoggerLevel::Disabled);

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
  ngLogger.setLevel(LoggerLevel::Disabled);

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
  ngLogger.setLevel(LoggerLevel::Disabled);

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