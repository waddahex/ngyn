#include <ngyn/core/window.hpp>
#include <ngyn/core/time.hpp>
#include <ngyn/core/input.hpp>

#include <ngyn/util/logger.hpp>
#include <memory>
#include <vector>

#include <ngyn/renderers/quad_renderer.hpp>

using namespace ngyn;

int main()
{
  logger.setFormat("$T");

  Window window({
    .monitor = 1
  });

  Time time;
  Input input;

  std::vector<QuadInstanceData> instances;

  instances.push_back(QuadInstanceData());
  auto &data = instances.back();

  // LOGGER_DEBUG(data);
  instances.erase(instances.begin());
  // LOGGER_DEBUG(data);

  while(window.isOpen())
  {
    window.handleEvents();
    time.update();
    input.update(window);
    window.clear();

    window.swapBuffers();
  }
}