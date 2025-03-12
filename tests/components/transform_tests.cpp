#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <ngyn/ngyn.hpp>

using namespace ngyn;

TEST_CASE("Initialization")
{
  SUBCASE("Should have model different than default")
  {
    Transform transform{{
      .position = glm::vec2(0.0f),
      .size = glm::vec2(100.0f),
      .scale = glm::vec2(1.0f),
      .rotation = 0.0f,
    }};

    CHECK(transform.model() != glm::mat4(1.0f));
  }

  SUBCASE("Should have zIndex different than the default")
  {
    Transform transform{{
      .layer = 2
    }};

    CHECK(transform.zIndex() != 0.0f);
  }
}

TEST_CASE("Updates")
{
  SUBCASE("Model and position should be updated")
  {
    Transform transform{{
      .position = glm::vec2(0.0f)
    }};

    auto model = transform.model();
    auto position = transform.position();

    transform.setPosition(glm::vec2(123.0f));

    CHECK(position != transform.position());
    CHECK(model != transform.model());
  }

  SUBCASE("Model and size should be updated")
  {
    Transform transform{{
      .size = glm::vec2(0.0f)
    }};

    auto model = transform.model();
    auto size = transform.size();

    transform.setSize(glm::vec2(123.0f));

    CHECK(size != transform.size());
    CHECK(model != transform.model());
  }

  SUBCASE("Model and scale should be updated")
  {
    Transform transform{{
      .size = glm::vec2(1.0f),
      .scale = glm::vec2(1.0f)
    }};

    auto model = transform.model();
    auto scale = transform.scale();

    transform.setScale(glm::vec2(123.0f));

    CHECK(scale != transform.scale());
    CHECK(model != transform.model());
  }

  SUBCASE("Model and rotation should be updated")
  {
    Transform transform{{
      .size = glm::vec2(1.0f),
      .rotation = 0.0f
    }};

    auto model = transform.model();
    auto rotation = transform.rotation();

    transform.setRotation(45.0f);

    CHECK(rotation != transform.rotation());
    CHECK(model != transform.model());
  }

  SUBCASE("LayerMode and zIndex should be updated")
  {
    Transform transform{{
      .position = glm::vec2(2.0f),
      .size = glm::vec2(3.0f),
      .layerMode = Transform::LayerMode::Simple,
      .layer = 1
    }};

    auto layerMode = transform.layerMode();
    auto zIndex = transform.zIndex();

    transform.setLayerMode(Transform::LayerMode::Dynamic);

    CHECK(layerMode != transform.layerMode());
    CHECK(zIndex != transform.zIndex());
  }

  SUBCASE("zIndex and layer should be updated")
  {
    Transform transform{{
      .position = glm::vec2(2.0f),
      .size = glm::vec2(3.0f),
      .layerMode = Transform::LayerMode::Dynamic,
      .layer = 1
    }};

    auto layer = transform.layer();
    auto zIndex = transform.zIndex();

    transform.setLayer(2);

    CHECK(layer != transform.layer());
    CHECK(zIndex != transform.zIndex());
  }
}