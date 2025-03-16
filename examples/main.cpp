#include <ngyn/ngyn.hpp>

using namespace ngyn;

Sprite s1;
Sprite s2;

Text t1;
Text t2;
Text t3;
Text t4;

std::string value1 = "Red.x < Blue.x + Blue.width";
std::string value2 = "Red.x + Red.width > Blue.x";
std::string value3 = "Red.y < Blue.y + Blue.height";
std::string value4 = "Red.y + Red.height > Blue.y";

class Game : public Engine
{
  public:
  Game() : Engine({
    .window = Window{{
      .resizable = true,
      .monitor = 1
    }}
  }){};

  virtual void onSetup()
  {
    ngLogger.setFormat("HH:mm:ss");

    auto font = ResourcesManager::addResource<Font>("arial", Font{{
      .path = "data/fonts/dogica.ttf",
      .name = "arial",
      .size = 16,
      .pixelated = true
    }});

    auto camera = ResourcesManager::addResource<Camera>("main_camera", Camera{{
      .position = glm::vec2(0.0f),
      .resolution = window.resolution()
    }});

    auto quadRenderer = ResourcesManager::addResource<QuadRenderer>("quad_renderer", QuadRenderer{});

    s1 = Sprite(Sprite::CreateInfo{
      .frame = {
        .color = Color(255, 0, 0)
      },
      .transform = {
        .position = glm::vec2(0.0f, 200.0f),
        .size = glm::vec2(100.0f)
      },
      .renderer = quadRenderer,
      .camera = camera,
    });

    s2 = Sprite(Sprite::CreateInfo{
      .frame = {
        .color = Color(255, 255, 0)
      },
      .transform = {
        .position = glm::vec2(200.0f),
        .size = glm::vec2(100.0f)
      },
      .renderer = quadRenderer,
      .camera = camera,
    });

    s1.instantiate();
    s2.instantiate();

    t1 = Text(Text::CreateInfo{
      .font = font,
      .camera = camera,
      .renderer = quadRenderer,
      .value = value1,
      .color = Color(255)
    });

    t2 = Text(Text::CreateInfo{
      .font = font,
      .camera = camera,
      .renderer = quadRenderer,
      .position = glm::vec2(0.0f, 20.0f),
      .value = value2,
      .color = Color(255)
    });

    t3 = Text(Text::CreateInfo{
      .font = font,
      .camera = camera,
      .renderer = quadRenderer,
      .position = glm::vec2(0.0f, 20.0f * 2.0f),
      .value = value3,
      .color = Color(255)
    });

    t4 = Text(Text::CreateInfo{
      .font = font,
      .camera = camera,
      .renderer = quadRenderer,
      .position = glm::vec2(0.0f, 20.0f * 3.0f),
      .value = value4,
      .color = Color(255)
    });

    t1.instantiate();
    t2.instantiate();
    t3.instantiate();
    t4.instantiate();

    quadRenderer.lock().get()->setup();
  };

  bool isColliding(Transform &transform1, Transform &transform2)
  {
    // Check if x1 < x2 + w2 rect1 right edge
    bool res1 = transform1.position().x < transform2.position().x + transform2.size().x;
    t1.setValue(std::format("{}: {}", value1, res1));
    // Check if x2 > x1 + w1 rect1 left edge
    bool res2 = transform1.position().x + transform1.size().x > transform2.position().x;
    t2.setValue(std::format("{}: {}", value2, res2));
    // Check if y1 < y2 + h2 rect1 top edge
    bool res3 = transform1.position().y < transform2.position().y + transform2.size().y;
    t3.setValue(std::format("{}: {}", value3, res3));
    // Check if y2 < y1 + h1 rect1 bottom edge
    bool res4 = transform1.position().y + transform1.size().y > transform2.position().y;
    t4.setValue(std::format("{}: {}", value4, res4));

    return res1 && res2 && res3 && res4;
  }

  virtual void onUpdate()
  {
    glm::vec2 velocity(0.0f);
    float speed = 100.0f;

    if(ngInput.held("KEY_A"))
    {
      velocity.x -= speed * ngTime.deltaTime();
    }

    if(ngInput.held("KEY_D"))
    {
      velocity.x += speed * ngTime.deltaTime();
    }

    if(ngInput.held("KEY_W"))
    {
      velocity.y -= speed * ngTime.deltaTime();
    }

    if(ngInput.held("KEY_S"))
    {
      velocity.y += speed * ngTime.deltaTime();
    }

    if(isColliding(s1.transform, s2.transform))
    {
      s1.frame.setColor(Color(255, 0, 0, 100));
    }
    else
    {
      s1.frame.setColor(Color(255, 0, 0));
    }

    t1.update();
    t2.update();
    t3.update();
    t4.update();

    s1.moveBy(velocity);
    s1.update();
  };

  virtual void onRender()
  {
    auto quadRenderer = ResourcesManager::getResource<QuadRenderer>("quad_renderer");
    quadRenderer.lock().get()->render();
  };
};

int main()
{
  Game game;
  game.run();
}