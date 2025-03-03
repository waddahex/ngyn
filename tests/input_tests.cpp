#include <catch2/catch_test_macros.hpp>
#include <ngyn/util/logger.hpp>
#include <ngyn/core/window.hpp>
#include <ngyn/core/input.hpp>

#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN
#undef APIENTRY
#include <windows.h>

#endif

using namespace ngyn;

void simulateKeyPress(int virtualKey, const InputState &state)
{
  #ifdef _WIN32

  INPUT ip = {0};
  ip.type = INPUT_KEYBOARD;
  ip.ki.wVk = virtualKey;

  if(state.pressed)
  {
    // Press input
    SendInput(1, &ip, sizeof(INPUT));
  }

  if(state.released)
  {
    ip.ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(1, &ip, sizeof(INPUT));
  }

  #endif
}

void simulateMouseClick(const Window &window, const InputState &state)
{
  #ifdef _WIN32

  HWND handle = FindWindow(nullptr, window.title.c_str());

  LPARAM lParam = MAKELPARAM(100, 100);

  if(state.pressed)
  {
    SendMessage(handle, WM_LBUTTONDOWN, MK_LBUTTON, lParam);
  }

  if(state.released)
  {
    SendMessage(handle, WM_LBUTTONUP, MK_LBUTTON, lParam);
  }

  #endif
}

TEST_CASE("Keyboard input", "[input]")
{
  Window window(WindowCreateInfo{});
  Input input(InputCreateInfo{
    .usePrintableKeys = true,
    .useFunctionalKeys = true,
  });
  int virtualKeyA = 0x41;

  SECTION("Input pressed action KEY_A should be true")
  {
    simulateKeyPress(virtualKeyA, {.pressed = true});
    window.handleEvents();
    input.update(window);

    REQUIRE(input.pressed("KEY_A"));
  }

  SECTION("Input held action KEY_A should be true")
  {
    simulateKeyPress(virtualKeyA, {.pressed = true});
    window.handleEvents();
    input.update(window);

    window.handleEvents();
    input.update(window);

    REQUIRE(input.held("KEY_A"));
  }

  SECTION("Input released action KEY_A should be true")
  {
    simulateKeyPress(virtualKeyA, {.pressed = true});
    window.handleEvents();
    input.update(window);

    simulateKeyPress(virtualKeyA, {.released = true});
    window.handleEvents();
    input.update(window);

    REQUIRE(input.released("KEY_A"));
  }

  SECTION("Input released action KEY_A should be false if pressed or held wasn't true")
  {
    simulateKeyPress(virtualKeyA, {.released = true});
    window.handleEvents();
    input.update(window);

    REQUIRE(!input.released("KEY_A"));
  }
}

TEST_CASE("Mouse input", "[input]")
{
  Window window(WindowCreateInfo{});
  Input input(InputCreateInfo{
    .useMouseButtons = true,
  });

  SECTION("Input pressed action MOUSE_BUTTON_LEFT should be true")
  {
    simulateMouseClick(window, {.pressed = true});
    window.handleEvents();
    input.update(window);

    REQUIRE(input.pressed("MOUSE_BUTTON_LEFT"));
  }

  SECTION("Input held action MOUSE_BUTTON_LEFT should be true")
  {
    simulateMouseClick(window, {.pressed = true});
    window.handleEvents();
    input.update(window);

    window.handleEvents();
    input.update(window);

    REQUIRE(input.held("MOUSE_BUTTON_LEFT"));
  }

  SECTION("Input held action MOUSE_BUTTON_LEFT should be true")
  {
    simulateMouseClick(window, {.pressed = true});
    window.handleEvents();
    input.update(window);

    simulateMouseClick(window, {.released = true});
    window.handleEvents();
    input.update(window);

    REQUIRE(input.released("MOUSE_BUTTON_LEFT"));
  }

  SECTION("Input held action MOUSE_BUTTON_LEFT should be false if pressed or held wans't true")
  {
    simulateMouseClick(window, {.released = true});
    window.handleEvents();
    input.update(window);

    REQUIRE(!input.released("MOUSE_BUTTON_LEFT"));
  }
}
