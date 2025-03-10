#include <catch2/catch_test_macros.hpp>
#include <ngyn/ngyn.hpp>

using namespace ngyn;

TEST_CASE("All operations", "[resources_manager]")
{
  SECTION("Should return an empty storage")
  {
    auto storage = ResourcesManager::getStorage<int>();
    REQUIRE(storage.resources.empty());
  }

  SECTION("Should add a resource and return a valid weak_ptr")
  {
    auto ptr = ResourcesManager::addResource<int>("one", 1);
    auto storage = ResourcesManager::getStorage<int>();

    REQUIRE(!storage.resources.empty());
    REQUIRE(ptr.lock());
  }

  SECTION("Should return the correct resource")
  {
    ResourcesManager::addResource<int>("two", 2);

    auto resource = ResourcesManager::getResource<int>("two");
    int value = *resource.lock().get();

    REQUIRE(value == 2);
  }

  SECTION("Should return an invalid weak_ptr")
  {
    auto resource = ResourcesManager::getResource<int>("invalid_resource");

    REQUIRE(!resource.lock());
  }

  SECTION("Should make resource weak_ptr invalid")
  {
    auto ptr = ResourcesManager::addResource<float>("0.5", 0.5f);
    ResourcesManager::removeResource<float>("0.5");

    REQUIRE(!ptr.lock());
  }

  SECTION("Should remove resource from storage")
  {
    ResourcesManager::addResource<float>("0.4", 0.4f);
    ResourcesManager::removeResource<float>("0.4");

    auto storage = ResourcesManager::getStorage<float>();

    REQUIRE(storage.resources.find("0.4") == storage.resources.end());
  }
}