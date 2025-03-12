#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <ngyn/ngyn.hpp>

using namespace ngyn;

TEST_CASE("Initialization")
{
  SUBCASE("Projection and view should match")
  {
    glm::vec2 position(0.0f);
    glm::vec2 resolution(1280.0f, 720.0f);

    Camera camera{{
      .position = position,
      .resolution = resolution
    }};

    auto projection = glm::ortho(0.0f, resolution.x, resolution.y, 0.0f, -1.0f, 1.0f);
    auto view = glm::translate(glm::mat4(1.0f), -glm::vec3(position, 0.0f));

    CHECK(projection == camera.projection());
    CHECK(view == camera.view());
  }
}

TEST_CASE("Update methods")
{
  SUBCASE("View and position should be updated")
  {
    glm::vec2 position(0.0f);
    glm::vec2 resolution(1280.0f, 720.0f);

    Camera camera{{
      .position = position,
      .resolution = resolution
    }};

    glm::vec2 newPosition = glm::vec2(128.0f);

    camera.setPosition(newPosition);

    auto newView = glm::translate(glm::mat4(1.0f), -glm::vec3(newPosition, 0.0f));

    CHECK(newPosition == camera.position());
    CHECK(newView == camera.view());
  }

  SUBCASE("Projection and resolution should be updated")
  {
    glm::vec2 position(0.0f);
    glm::vec2 resolution(1280.0f, 720.0f);

    Camera camera{{
      .position = position,
      .resolution = resolution
    }};

    glm::vec2 newResolution = glm::vec2(1920.0f, 1080.0f);

    camera.setResolution(newResolution);

    auto newProjection = glm::ortho(0.0f, newResolution.x, newResolution.y, 0.0f, -1.0f, 1.0f);

    CHECK(newResolution == camera.resolution());
    CHECK(newProjection == camera.projection());
  }
}