#pragma once
#include <cstdint>

#include "Graphic.h"
#include "Memory.h"
#include "RandomGenerator.h"
#include "Register.h"
#include "Stack.h"

class Chip8
{
  Memory<uint8_t> memory;
  Graphic<uint32_t> graphic;
  Register<uint16_t> programCounter;
  Stack<uint16_t> stack;
  RandomGenerator<uint8_t> random;
  uint16_t opcode = 0;

  void loadFont() const;

public:
  Chip8();
  ~Chip8();
  void loadRom(const char* filePath) const;

  // Clear the display
  void OP_00E0() const;

  // Return from a subroutine
  void OP_00EE();

  // Jump to location nnn
  void OP_1nnn();

  // Call subroutine at nnn
  void OP_2nnn();
};
