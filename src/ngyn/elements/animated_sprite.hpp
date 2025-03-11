#pragma once

#include "../../ngynpch.hpp"

#include "../util/random.hpp"
#include "sprite.hpp"
#include "../core/time.hpp"

namespace ngyn
{
  class AnimatedSprite : public Sprite
  {
    public:
    struct Animation
    {
      std::string name = "animation";
      float duration = 0.0f;
      float delay = 0.0f;
      std::vector<int> frames;
      bool repeat = false;
    };

    public:
    AnimatedSprite(void) = default;
    AnimatedSprite(CreateInfo createInfo) :
      Sprite(createInfo),
      _uuid(random::uuid())
    {};

    void play(const std::string &name);
    void stop();
    void toggle();
    void addAnimation(const Animation &animation);

    void update();
    void instantiate();

    private:
    Animation _currentAnimation;
    int _currentFrame;
    std::unordered_map<std::string, Animation> _animations;
    bool _playing;
    std::string _uuid;

    void animate();
    void reset();
  };
};