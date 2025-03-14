#include "animated_sprite.hpp"

using namespace ngyn;

AnimatedSprite::AnimatedSprite(CreateInfo createInfo) :
  Sprite(createInfo),
  _playing(false),
  _currentFrame(0)
{
}

const std::string &AnimatedSprite::uuid()
{
  return _uuid;
}

const bool &AnimatedSprite::playing()
{
  return _playing;
}

const std::string &AnimatedSprite::currentAnimation()
{
  return _currentAnimation;
}

const std::unordered_map<std::string, AnimatedSprite::Animation> &AnimatedSprite::animations()
{
  return _animations;
}

void AnimatedSprite::play(const std::string &name)
{
  if(_animations.find(name) == _animations.end()) return;

  _playing = true;

  // This prevents reseting the animation
  if(_currentAnimation == name) return;

  _currentAnimation = name;

  reset();
}

void AnimatedSprite::instantiate()
{
  _uuid = random::uuid();
  Sprite::instantiate();
}

void AnimatedSprite::stop()
{
  _playing = false;

  if(_animations.find(_currentAnimation) == _animations.end()) return;

  reset();
}

void AnimatedSprite::update()
{
  if(_playing && !_animations.empty())
  {
    animate();
  }

  Sprite::update();
}

void AnimatedSprite::animate()
{
  auto animation = _animations[_currentAnimation];

  float frameDuration = animation.duration / animation.frames.size() / 1000.0f;

  // Checks if the frameDuration has passed
  if(ngTime.hasPassed(_uuid, frameDuration))
  {
    _currentFrame++;

    // Prevents frames out of bounds
    if(_currentFrame > animation.frames.size() -1)
    {
      // Stops animation if shouldn't repeat
      if(!animation.repeat)
      {
        _playing = false;
      }

      _currentFrame = 0;
    }

    frame.setIndex(animation.frames[_currentFrame]);
  }
}

void AnimatedSprite::reset()
{
  _currentFrame = 0;

  // Resets animation timer
  ngTime.resetTimepoint(_uuid);

  auto animation = _animations[_currentAnimation];

  frame.setIndex(animation.frames[_currentFrame]);
}

void AnimatedSprite::toggle()
{
  _playing = !_playing;
}

void AnimatedSprite::setAnimation(const Animation &animation)
{
  ASSERT(!animation.name.empty(), "Animation must have a name");

  _animations[animation.name] = animation;
}