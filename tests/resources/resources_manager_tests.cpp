#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <ngyn/ngyn.hpp>

using namespace ngyn;

TEST_CASE("All operations")
{
  SUBCASE("Should return an empty storage")
  {
    auto storage = ResourcesManager::getStorage<int>();
    CHECK(storage.resources.empty());
  }

  SUBCASE("Should add a resource and return a valid weak_ptr")
  {
    auto ptr = ResourcesManager::addResource<int>("one", 1);
    auto storage = ResourcesManager::getStorage<int>();

    CHECK(!storage.resources.empty());
    CHECK(ptr.lock());
  }

  SUBCASE("Should return the correct resource")
  {
    ResourcesManager::addResource<int>("two", 2);

    auto resource = ResourcesManager::getResource<int>("two");
    int value = *resource.lock().get();

    CHECK(value == 2);
  }

  SUBCASE("Should return an invalid weak_ptr")
  {
    auto resource = ResourcesManager::getResource<int>("invalid_resource");

    CHECK(!resource.lock());
  }

  SUBCASE("Should make resource weak_ptr invalid")
  {
    auto ptr = ResourcesManager::addResource<float>("0.5", 0.5f);
    ResourcesManager::removeResource<float>("0.5");

    CHECK(!ptr.lock());
  }

  SUBCASE("Should remove resource from storage")
  {
    ResourcesManager::addResource<float>("0.4", 0.4f);
    ResourcesManager::removeResource<float>("0.4");

    auto storage = ResourcesManager::getStorage<float>();

    CHECK(storage.resources.find("0.4") == storage.resources.end());
  }
}