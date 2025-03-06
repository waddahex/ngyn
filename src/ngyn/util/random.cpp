#include "random.hpp"

int ngyn::random::getInteger(int min, int max)
{
  std::random_device rd;
  std::mt19937_64 mt(rd());
  std::uniform_int_distribution<int> dist(min, max);

  return dist(mt);
}

float ngyn::random::getFloat(float min, float max)
{
  std::random_device rd;
  std::mt19937_64 mt(rd());
  std::uniform_real_distribution<float> dist(min, max);

  return dist(mt);
}