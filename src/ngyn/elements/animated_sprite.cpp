#include "animated_sprite.hpp"

using namespace ngyn;

void AnimatedSprite::play(const std::string &name)
{
  if(_animations.find(name) == _animations.end()) return;

  _currentAnimation = _animations[name];
  _playing = true;

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
  _currentFrame = 0;

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
  float frameDuration = _currentAnimation.duration / _currentAnimation.frames.size() / 1000.0f;

  // Checks if the frameDuration has passed
  if(Time::hasPassed(_uuid, frameDuration))
  {
    // Prevents frames out of bounds
    if(_currentFrame > _currentAnimation.frames.size() -1)
    {
      // Stops animation if shouldn't repeat
      if(!_currentAnimation.repeat)
      {
        _playing = false;
      }

      _currentFrame = 0;
    }

    LOGGER_DEBUG(_currentAnimation.frames[_currentFrame]);
    frame.setIndex(_currentAnimation.frames[_currentFrame]);

    _currentFrame++;

  }
}

void AnimatedSprite::reset()
{
  _currentFrame = 0;

  // Resets animation timer
  Time::resetTimepoint(_uuid);

  frame.setIndex(_currentAnimation.frames[_currentFrame]);
}

void AnimatedSprite::toggle()
{
  _playing = !_playing;
}

void AnimatedSprite::addAnimation(const Animation &animation)
{
  ASSERT(!animation.name.empty(), "Animation must have a name");

  _animations[animation.name] = animation;
}