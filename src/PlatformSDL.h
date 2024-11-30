#pragma once

#include <SDL2/SDL.h>

#include "Keypad.h"

class PlatformSDL
{
  SDL_Window* window{};
  SDL_Renderer* renderer{};
  SDL_Texture* texture{};

public:
  PlatformSDL();
  PlatformSDL(int graphicWidth, int graphicHeight, int scale);
  ~PlatformSDL();
  void update(const void* buffer, int pitch) const;
  static bool ProcessInput(Keypad& keypad);
};
