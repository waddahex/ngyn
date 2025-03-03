#include <rapidjson/document.h>
#include <ngyn/util/logger.hpp>

using namespace ngyn;

int main()
{
  LOGGER_DEBUG("Debug log");
  LOGGER_WARNING("Warning log");
  LOGGER_ERROR("Error log");
  logger.setLevel(LoggerLevel::Warning);
  LOGGER_DEBUG("Debug log 2");
  LOGGER_WARNING("Warning log 2");
  LOGGER_ERROR("Error log 2");
  logger.setLevel(LoggerLevel::Error);
  LOGGER_DEBUG("Debug log 3");
  LOGGER_WARNING("Warning log 3");
  LOGGER_ERROR("Error log 3");
}