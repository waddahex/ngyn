#define DOCTEST_CONFIG_IMPLEMENT
#include <doctest/doctest.h>

#include <ngyn/ngyn.hpp>

using namespace ngyn;

std::string dataPath = "data";

Window window{{.title = "ngyntest"}};

int main(int argc, char **argv)
{
  window.open();
  window.loadGL();

  if(argc > 1)
  {
    dataPath = (std::filesystem::path(argv[1]) / "data").string();
  }

  doctest::Context context;

  context.applyCommandLine(argc, argv);

  int res = context.run();

  if(context.shouldExit()) return res;

  return res;
}

using namespace ngyn;

int virtualKeyA = 0x41;

void simulateKeyPress(int virtualKey, const Input::State &state)
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

void simulateMouseClick(const Input::State &state)
{
  #ifdef _WIN32

  HWND handle = FindWindow(nullptr, "ngyntest");

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


TEST_CASE("Keyboard input")
{
  SUBCASE("Input pressed action KEY_A should be true")
  {
    simulateKeyPress(virtualKeyA, {.pressed = true});
    window.handleEvents();
    ngInput.update(window.handle());

    CHECK(ngInput.pressed("KEY_A"));
  }

  SUBCASE("Input held action KEY_A should be true")
  {
    simulateKeyPress(virtualKeyA, {.pressed = true});
    window.handleEvents();
    ngInput.update(window.handle());

    window.handleEvents();
    ngInput.update(window.handle());

    CHECK(ngInput.held("KEY_A"));
  }

  SUBCASE("Input released action KEY_A should be true")
  {
    simulateKeyPress(virtualKeyA, {.pressed = true});
    window.handleEvents();
    ngInput.update(window.handle());

    simulateKeyPress(virtualKeyA, {.released = true});
    window.handleEvents();
    ngInput.update(window.handle());

    CHECK(ngInput.released("KEY_A"));
  }

  SUBCASE("Input released action KEY_A should be false if pressed or held wasn't true")
  {
    simulateKeyPress(virtualKeyA, {.released = true});
    window.handleEvents();
    ngInput.update(window.handle());

    CHECK(!ngInput.released("KEY_A"));
  }
}

TEST_CASE("Mouse input")
{
  SUBCASE("Input pressed action MOUSE_BUTTON_LEFT should be true")
  {
    simulateMouseClick({.pressed = true});
    window.handleEvents();
    ngInput.update(window.handle());

    CHECK(ngInput.pressed("MOUSE_BUTTON_LEFT"));
  }

  SUBCASE("Input held action MOUSE_BUTTON_LEFT should be true")
  {
    simulateMouseClick({.pressed = true});
    window.handleEvents();
    ngInput.update(window.handle());

    window.handleEvents();
    ngInput.update(window.handle());

    CHECK(ngInput.held("MOUSE_BUTTON_LEFT"));
  }

  SUBCASE("Input held action MOUSE_BUTTON_LEFT should be true")
  {
    simulateMouseClick({.pressed = true});
    window.handleEvents();
    ngInput.update(window.handle());

    simulateMouseClick({.released = true});
    window.handleEvents();
    ngInput.update(window.handle());

    CHECK(ngInput.released("MOUSE_BUTTON_LEFT"));
  }

  SUBCASE("Input held action MOUSE_BUTTON_LEFT should be false if pressed or held wans't true")
  {
    simulateMouseClick({.released = true});
    window.handleEvents();
    ngInput.update(window.handle());

    CHECK(!ngInput.released("MOUSE_BUTTON_LEFT"));
  }
}

TEST_CASE("Multiple actions")
{
  SUBCASE("Input held should be true")
  {
    simulateKeyPress(virtualKeyA, {.pressed = true});

    window.handleEvents();
    ngInput.update(window.handle());

    window.handleEvents();
    ngInput.update(window.handle());

    CHECK(ngInput.held("INVALID_KEY", "GAMEPAD_BUTTON_A", "KEY_A", "MOUSE_BUTTON_LEFT"));

    simulateKeyPress(virtualKeyA, {.released = true});

    simulateMouseClick({.pressed = true});

    window.handleEvents();
    ngInput.update(window.handle());

    window.handleEvents();
    ngInput.update(window.handle());

    CHECK(ngInput.held("INVALID_KEY", "GAMEPAD_BUTTON_A", "KEY_A", "MOUSE_BUTTON_LEFT"));

    simulateMouseClick({.released = true});
  }

  SUBCASE("Input pressed should be true")
  {
    simulateKeyPress(virtualKeyA, {.pressed = true});

    window.handleEvents();
    ngInput.update(window.handle());
    
    CHECK(ngInput.pressed("INVALID_KEY", "GAMEPAD_BUTTON_A", "KEY_A", "MOUSE_BUTTON_LEFT"));

    simulateKeyPress(virtualKeyA, {.released = true});

    window.handleEvents();
    ngInput.update(window.handle());

    simulateMouseClick({.pressed = true});

    window.handleEvents();
    ngInput.update(window.handle());

    CHECK(ngInput.pressed("INVALID_KEY", "GAMEPAD_BUTTON_A", "KEY_A", "MOUSE_BUTTON_LEFT"));
  }

  SUBCASE("Input released should be true")
  {
    simulateKeyPress(virtualKeyA, {.pressed = true});

    window.handleEvents();
    ngInput.update(window.handle());

    simulateKeyPress(virtualKeyA, {.released = true});

    window.handleEvents();
    ngInput.update(window.handle());

    CHECK(ngInput.released("INVALID_KEY", "GAMEPAD_BUTTON_A", "KEY_A", "MOUSE_BUTTON_LEFT"));

    simulateMouseClick({.pressed = true});

    window.handleEvents();
    ngInput.update(window.handle());

    simulateMouseClick({.released = true});

    window.handleEvents();
    ngInput.update(window.handle());

    CHECK(ngInput.released("INVALID_KEY", "GAMEPAD_BUTTON_A", "KEY_A", "MOUSE_BUTTON_LEFT"));
  }
}