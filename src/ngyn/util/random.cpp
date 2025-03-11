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

std::string ngyn::random::uuid()
{
  std::random_device rd;
  std::mt19937_64 mt(rd());
  std::uniform_int_distribution<uint64_t> dist;

  auto part1 = dist(mt);
  auto part2 = dist(mt);

  std::stringstream sstream;

  sstream << std::hex << std::setw(16) << std::setfill('0') << part1;
  sstream << std::hex << std::setw(16) << std::setfill('0') << part2;

  return strings::uppercase(sstream.str());
}