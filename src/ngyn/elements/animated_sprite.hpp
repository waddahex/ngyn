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
    AnimatedSprite(CreateInfo createInfo);

    const std::string &uuid();
    const bool &playing();
    const std::string &currentAnimation();
    const std::unordered_map<std::string, Animation> &animations();

    void play(const std::string &name);
    void stop();
    void toggle();
    void setAnimation(const Animation &animation);

    void update();
    void instantiate();

    private:
    int _currentFrame;
    bool _playing;
    std::string _uuid;
    std::string _currentAnimation;
    std::unordered_map<std::string, Animation> _animations;

    void animate();
    void reset();
  };
};