#include <catch2/catch_test_macros.hpp>
#include <ngyn/ngyn.hpp>

using namespace ngyn;

TEST_CASE("Initialization", "[transform]")
{
  SECTION("Should have model different than default")
  {
    Transform transform{{
      .position = glm::vec2(0.0f),
      .size = glm::vec2(100.0f),
      .scale = glm::vec2(1.0f),
      .rotation = 0.0f,
    }};

    REQUIRE(transform.model() != glm::mat4(1.0f));
  }

  SECTION("Should have zIndex different than the default")
  {
    Transform transform{{
      .layer = 2
    }};

    REQUIRE(transform.zIndex() != 0.0f);
  }
}

TEST_CASE("Updates", "[transform]")
{
  SECTION("Model and position should be updated")
  {
    Transform transform{{
      .position = glm::vec2(0.0f)
    }};

    auto model = transform.model();
    auto position = transform.position();

    transform.setPosition(glm::vec2(123.0f));

    REQUIRE(position != transform.position());
    REQUIRE(model != transform.model());
  }

  SECTION("Model and size should be updated")
  {
    Transform transform{{
      .size = glm::vec2(0.0f)
    }};

    auto model = transform.model();
    auto size = transform.size();

    transform.setSize(glm::vec2(123.0f));

    REQUIRE(size != transform.size());
    REQUIRE(model != transform.model());
  }

  SECTION("Model and scale should be updated")
  {
    Transform transform{{
      .size = glm::vec2(1.0f),
      .scale = glm::vec2(1.0f)
    }};

    auto model = transform.model();
    auto scale = transform.scale();

    transform.setScale(glm::vec2(123.0f));

    REQUIRE(scale != transform.scale());
    REQUIRE(model != transform.model());
  }

  SECTION("Model and rotation should be updated")
  {
    Transform transform{{
      .size = glm::vec2(1.0f),
      .rotation = 0.0f
    }};

    auto model = transform.model();
    auto rotation = transform.rotation();

    transform.setRotation(45.0f);

    REQUIRE(rotation != transform.rotation());
    REQUIRE(model != transform.model());
  }

  SECTION("LayerMode and zIndex should be updated")
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

    REQUIRE(layerMode != transform.layerMode());
    REQUIRE(zIndex != transform.zIndex());
  }

  SECTION("zIndex and layer should be updated")
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

    REQUIRE(layer != transform.layer());
    REQUIRE(zIndex != transform.zIndex());
  }
}