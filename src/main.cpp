#include <iostream>

#include "Chip8.h"
#include "PlatformSDL.h"

int main(const int argc, char* argv[])
{
  if (argc != 2)
  {
    std::cerr << "Usage: " << argv[0] << " <ROM>\n";
    std::exit(EXIT_FAILURE);
  }

  const std::string romFilename = argv[1];

  constexpr unsigned int SCALE = 15;
  constexpr unsigned int CYCLES_PER_SECOND = 1082; // Emulated CPU cycles per second
  constexpr unsigned int FRAME_RATE = 30;


  try
  {
    Chip8 chip8(romFilename);
    const PlatformSDL platform_sdl(GRAPHIC_WIDTH, GRAPHIC_HEIGHT, SCALE);
    bool quit = false;

    while (!quit)
    {
      const uint32_t start_time = SDL_GetTicks();

      quit = PlatformSDL::ProcessInput(chip8.getKeypad());

      for (int i = 0; i < (CYCLES_PER_SECOND / FRAME_RATE); ++i)
      {
        chip8.Cycle();
      }
      // Update Display at 60 Hz
      platform_sdl.update(chip8.getBuffer(), sizeof(uint32_t) * GRAPHIC_WIDTH);

      // Wait for the next frame if needed
      if (const uint32_t frame_time = SDL_GetTicks() - start_time; frame_time < (1000 / FRAME_RATE))
      {
        SDL_Delay((1000 / FRAME_RATE) - frame_time);
      }
    }
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << '\n';
    return EXIT_FAILURE;
  }

  return 0;
}
