#include "time.hpp"

ngyn::Time::Time()
{
  this->timePoint = std::chrono::high_resolution_clock::now();
  this->deltaTime       = 0.0f;
  this->lastTime        = 0.0f;
  this->fps             = 0.0f;
  this->ms              = 0.0f;
  this->updateDelay     = 1.0f;
  this->deltaTimeSum    = 0.0f;
  this->deltaTimeCount  = 0;
  this->justUpdated     = false;
  this->lastUpdatedTime = 0.0f;
}

void ngyn::Time::update()
{
  float currentTime = this->getTime();
  this->deltaTime = currentTime - this->lastTime;
  this->lastTime  = currentTime;
  this->deltaTimeSum += this->deltaTime;
  this->deltaTimeCount++;
  this->justUpdated = false;

  // Update the counters if the updateDelay has passed
  if(currentTime - this->lastUpdatedTime >= this->updateDelay)
  {
    this->ms = 1000.0f * (this->deltaTimeSum / this->deltaTimeCount);
    this->fps = 1.0f / (this->deltaTimeSum / this->deltaTimeCount);

    this->justUpdated     = true;
    this->deltaTimeCount  = 0;
    this->deltaTimeSum    = 0.0f;
    this->lastUpdatedTime = currentTime;
  }
}

float ngyn::Time::getTime()
{
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<float> seconds = end - this->timePoint;

  return seconds.count();
}
