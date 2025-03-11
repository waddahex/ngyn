#pragma once

#include "../../ngynpch.hpp"
#include "../util/logger.hpp"

namespace ngyn
{
  typedef std::chrono::high_resolution_clock::time_point TimePoint;

  class Time
  {
    public:
    Time();

    const float &ms();
    const float &fps();
    const float &deltaTime();
    const bool &justUpdated();

    void update();

    /**
     * Get elapsed time in seconds
     */
    float getTime();
    static bool hasPassed(const std::string &name, float seconds, bool reset = true);
    static void resetTimepoint(const std::string &name);

    private:
    float _ms;
    float _fps;
    float _deltaTime;
    bool _justUpdated;
    float _lastTime;
    float _deltaTimeSum; // Total deltaTime before updating
    int _deltaTimeCount; // Frames
    float _updateDelay; // Time between updates
    float _lastUpdatedTime; // Time of last update
    TimePoint _timePoint; // Starting time

    static std::unordered_map<std::string, TimePoint> _timePoints;
  };
};