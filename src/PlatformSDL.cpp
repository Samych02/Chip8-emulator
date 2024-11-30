#include "PlatformSDL.h"

#include <iostream>
#include <ostream>

PlatformSDL::PlatformSDL()
= default;

PlatformSDL::PlatformSDL(const int graphicWidth, const int graphicHeight, const int scale)
{
  if (SDL_Init(SDL_INIT_VIDEO) < 0 || SDL_Init(SDL_INIT_AUDIO) < 0)
  {
    throw std::runtime_error(std::string("Failed to initialize SDL ") + SDL_GetError());
  }

  window = SDL_CreateWindow("Chip 8", 0, 0, graphicWidth * scale, graphicHeight * scale, SDL_WINDOW_SHOWN);
  if (window == nullptr)
  {
    throw std::runtime_error(std::string("Failed to initialize window ") + SDL_GetError());
  }

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (renderer == nullptr)
  {
    SDL_DestroyWindow(window);
    throw std::runtime_error(std::string("Failed to initialize renderer ") + SDL_GetError());
  }

  texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, graphicWidth,
                              graphicHeight);
  if (texture == nullptr)
  {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    throw std::runtime_error(std::string("Failed to initialize texture ") + SDL_GetError());
  }
}

PlatformSDL::~PlatformSDL()
{
  SDL_DestroyTexture(texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

void PlatformSDL::update(const void* buffer, const int pitch) const
{
  SDL_UpdateTexture(texture, nullptr, buffer, pitch);
  SDL_RenderClear(renderer);
  SDL_RenderCopy(renderer, texture, nullptr, nullptr);
  SDL_RenderPresent(renderer);
}

bool PlatformSDL::ProcessInput(Keypad& keypad)
{
  bool quit = false;

  SDL_Event event;

  while (SDL_PollEvent(&event))
  {
    switch (event.type)
    {
    case SDL_QUIT:
      quit = true;
      break;

    case SDL_KEYDOWN:
      switch (event.key.keysym.sym)
      {
      case SDLK_ESCAPE:
        quit = true;
        break;

      case SDLK_x:
        keypad.pressKey(0x0u);
        break;

      case SDLK_1:
        keypad.pressKey(0x1u);
        break;

      case SDLK_2:
        keypad.pressKey(0x2u);
        break;

      case SDLK_3:
        keypad.pressKey(0x3u);
        break;

      case SDLK_q:
        keypad.pressKey(0x4u);
        break;

      case SDLK_w:
        keypad.pressKey(0x5u);
        break;

      case SDLK_e:
        keypad.pressKey(0x6u);
        break;

      case SDLK_a:
        keypad.pressKey(0x7u);
        break;

      case SDLK_s:
        keypad.pressKey(0x8u);
        break;

      case SDLK_d:
        keypad.pressKey(0x9u);
        break;

      case SDLK_z:
        keypad.pressKey(0xAu);
        break;

      case SDLK_c:
        keypad.pressKey(0xBu);
        break;

      case SDLK_4:
        keypad.pressKey(0xCu);
        break;

      case SDLK_r:
        keypad.pressKey(0xDu);
        break;

      case SDLK_f:
        keypad.pressKey(0xEu);
        break;

      case SDLK_v:
        keypad.pressKey(0xFu);
        break;

      default: break;
      }

    case SDL_KEYUP:
      switch (event.key.keysym.sym)
      {
      case SDLK_x:
        keypad.releaseKey(0x0u);
        break;

      case SDLK_1:
        keypad.releaseKey(0x1u);
        break;

      case SDLK_2:
        keypad.releaseKey(0x2u);
        break;

      case SDLK_3:
        keypad.releaseKey(0x3u);
        break;

      case SDLK_q:
        keypad.releaseKey(0x4u);
        break;

      case SDLK_w:
        keypad.releaseKey(0x5u);
        break;

      case SDLK_e:
        keypad.releaseKey(0x6u);
        break;

      case SDLK_a:
        keypad.releaseKey(0x7u);
        break;

      case SDLK_s:
        keypad.releaseKey(0x8u);
        break;

      case SDLK_d:
        keypad.releaseKey(0x9u);
        break;

      case SDLK_z:
        keypad.releaseKey(0xAu);
        break;

      case SDLK_c:
        keypad.releaseKey(0xBu);
        break;

      case SDLK_4:
        keypad.releaseKey(0xCu);
        break;

      case SDLK_r:
        keypad.releaseKey(0xDu);
        break;

      case SDLK_f:
        keypad.releaseKey(0xEu);
        break;

      case SDLK_v:
        keypad.releaseKey(0xFu);
        break;

      default: break;
      }

    default: break;
    }
  }

  return quit;
}
