#include "time.hpp"

using namespace ngyn;

Time::Time() :
  _ms(0.0f),
  _fps(0.0f),
  _deltaTime(0.0f),
  _justUpdated(false),
  _lastTime(0.0f),
  _deltaTimeSum(0.0f),
  _deltaTimeCount(0),
  _updateDelay(1.0f),
  _lastUpdatedTime(0.0f),
  _timePoint(std::chrono::high_resolution_clock::now())
{
}

void Time::update()
{
  float currentTime = getTime();
  _deltaTime = currentTime - _lastTime;
  _lastTime  = currentTime;
  _deltaTimeSum += _deltaTime;
  _deltaTimeCount++;
  _justUpdated = false;

  // Update the counters if the updateDelay has passed
  if(currentTime - _lastUpdatedTime >= _updateDelay)
  {
    _ms = 1000.0f * (_deltaTimeSum / _deltaTimeCount);
    _fps = 1.0f / (_deltaTimeSum / _deltaTimeCount);

    _justUpdated     = true;
    _deltaTimeCount  = 0;
    _deltaTimeSum    = 0.0f;
    _lastUpdatedTime = currentTime;
  }
}

float Time::getTime()
{
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<float> seconds = end - _timePoint;

  return seconds.count();
}

bool Time::hasPassed(const std::string &name, float seconds, bool reset)
{
  auto now = std::chrono::high_resolution_clock::now();

  if(_timePoints.find(name) == _timePoints.end())
  {
    _timePoints[name] = now;

    return false;
  }

  auto timePoint = _timePoints[name];

  std::chrono::duration<float> secondsPassed = now - timePoint;

  bool result = secondsPassed.count() >= seconds;

  if(result && reset)
  {
    _timePoints[name] = now;
  }

  return result;
}

void Time::resetTimepoint(const std::string &name)
{
  _timePoints[name] = std::chrono::high_resolution_clock::now();
}

const float &Time::ms()
{
  return _ms;
}

const float &Time::fps()
{
  return _fps;
}

const float &Time::deltaTime()
{
  return _deltaTime;
}

const bool &Time::justUpdated()
{
  return _justUpdated;
}