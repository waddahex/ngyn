#include <rapidjson/document.h>
#include <util/files.hpp>
#include <core/window.hpp>

using namespace ngyn;

int main()
{
  Window window(WindowCreateInfo{
    .configPath = "c:/dev/ngyn/data/windowConfig.json"
  });

  while(window.isOpen())
  {
    window.handleEvents();
    window.clear();
    window.swapBuffers();
  }

  window.destroy();
}