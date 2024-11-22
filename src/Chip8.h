#pragma once
#include <cstdint>

#include "Graphic.h"
#include "Memory.h"
#include "RandomGenerator.h"
#include "Register.h"
#include "Stack.h"

class Chip8
{
  std::array<Register<uint8_t>, 16> registers{};
  Memory<uint8_t> memory;
  Graphic<uint32_t> graphic;
  Register<uint16_t> programCounter;
  Register<uint16_t> index;
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

  // Skip next instruction if Vx = kk
  void OP_3xkk();

  // Skip next instruction if Vx ≠ kk
  void OP_4xkk();

  // Skip next instruction if Vx = Vy
  void OP_5xy0();

  // Set Vx = kk
  void OP_6xkk();

  // Set Vx = Vx + kk
  void OP_7xkk();

  // Set Vx = Vy
  void OP_8xy0();

  // Set Vx = Vx OR Vy
  void OP_8xy1();

  // Set Vx = Vx AND Vy
  void OP_8xy2();

  // Set Vx = Vx XOR Vy
  void OP_8xy3();

  // Set Vx = Vx + Vy, set VF = carry.
  // The values of Vx and Vy are added together. If the result is greater than 8 bits (i.e., > 255), VF is set to 1, otherwise 0. Only the lowest 8 bits of the result are kept, and stored in Vx.
  void OP_8xy4();

  // Set Vx = Vx - Vy, set VF = NOT borrow.
  // If Vx > Vy, then VF is set to 1, otherwise 0. Then Vy is subtracted from Vx, and the results stored in Vx.
  void OP_8xy5();

  // Set Vx = Vx SHR 1.
  // If the least-significant bit of Vx is 1, then VF is set to 1, otherwise 0. Then Vx is divided by 2.
  void OP_8xy6();

  // Set Vx = Vy - Vx, set VF = NOT borrow.
  // If Vy > Vx, then VF is set to 1, otherwise 0. Then Vx is subtracted from Vy, and the results stored in Vx.
  void OP_8xy7();

  // Set Vx = Vx SHL 1.
  // If the most-significant bit of Vx is 1, then VF is set to 1, otherwise to 0. Then Vx is multiplied by 2.
  void OP_8xyE();

  // Skip next instruction if Vx != Vy
  void OP_9xy0();

  // Set I = nnn
  void OP_Annn();

  // Jump to location nnn + V0
  void OP_Bnnn();

  // Set Vx = random byte AND kk
  void OP_Cxkk();

  // The interpreter reads n bytes from memory, starting at the address stored in I. These bytes are then displayed as
  // sprites on screen at coordinates (Vx, Vy). Sprites are XORed onto the existing screen. If this causes any pixels to
  // be erased, VF is set to 1, otherwise it is set to 0. If the sprite is positioned so part of it is outside
  // the coordinates of the display, it wraps around to the opposite side of the screen. See instruction 8xy3
  void OP_Dxyn();
};
