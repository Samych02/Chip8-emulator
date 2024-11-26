#pragma once
#include <cstdint>

class Keypad
{
  uint8_t keys[16]{};

public:
  void pressKey(const size_t key)
  {
    keys[key] = 1;
  }

  void releaseKey(const size_t key)
  {
    keys[key] = 0;
  }

  [[nodiscard]] bool isPressed(const size_t key) const
  {
    return keys[key];
  }

  void switchKey(const size_t key)
  {
    keys[key] = !keys[key];
  }
};
