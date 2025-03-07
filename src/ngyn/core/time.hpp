#pragma once

#include "../../ngynpch.hpp"

namespace ngyn
{
  typedef std::chrono::high_resolution_clock::time_point TimePoint;

  class Time
  {
    public:
    Time();

    float ms;
    float fps;
    float deltaTime;
    bool justUpdated;

    void update();

    /**
     * Get elapsed time in seconds
     */
    float getTime();

    private:
    float lastTime;
    float deltaTimeSum; // Total deltaTime before updating
    int deltaTimeCount; // Frames
    float updateDelay; // Time between updates
    float lastUpdatedTime; // Time of last update
    TimePoint timePoint; // Starting time
  };
};