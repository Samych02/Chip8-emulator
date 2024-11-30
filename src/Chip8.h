#pragma once
#include <cstdint>

#include "Graphic.h"
#include "Keypad.h"
#include "Memory.h"
#include "RandomGenerator.h"
#include "Register.h"
#include "Stack.h"

constexpr unsigned int RAM_SIZE = 4 * 1024;
constexpr unsigned int GRAPHIC_WIDTH = 64;
constexpr unsigned int GRAPHIC_HEIGHT = 32;
constexpr unsigned int STACK_SIZE = 16;
constexpr unsigned int STARTING_ADDRESS = 0x200;
constexpr unsigned int FONT_SET_START_ADDRESS = 0x50;

class Chip8
{
  std::array<Register<uint8_t>, 16> registers;
  Memory<uint8_t> memory;
  Graphic<uint32_t> graphic;
  Register<uint16_t> programCounter;
  Register<uint16_t> index;
  Register<uint8_t> delayTimer;
  Register<uint8_t> soundTimer;
  Stack<uint16_t> stack;
  Keypad keypad;
  RandomGenerator<uint8_t> random;
  uint16_t opcode;

  void loadFont() const;

  void loadRom(const std::string& filePath) const;

  // Clear the display
  void OP_00E0();

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

  // Skip next instruction if key with the value of Vx is pressed.
  void OP_Ex9E();

  // Skip next instruction if key with the value of Vx is not pressed.
  void OP_ExA1();

  // Set Vx = delay timer value
  void OP_Fx07();

  // Wait for a key press, store the value of the key in Vx
  void OP_Fx0A();

  // Set delay timer = Vx
  void OP_Fx15();

  // Set sound timer = Vx
  void OP_Fx18();

  // Set I = I + Vx
  void OP_Fx1E();

  // Set I = location of sprite for digit Vx
  void OP_Fx29();

  // The interpreter takes the decimal value of Vx, and places the hundreds digit in memory at location in I, the tens digit at location I+1, and the ones digit at location I+2
  void OP_Fx33();

  // Store registers V0 through Vx in memory starting at location I
  void OP_Fx55();

  // Read registers V0 through Vx from memory starting at location I
  void OP_Fx65();

  // cannot be set to static be it won't match type Chip8Function
  void OP_NULL()
  {
    /* Handle invalid opcodes */
  }

  // this type that we are defining (called Chip8Function) is a pointer to a function from class Chip8
  // this is useful to store pointers of class members inside a table to call these functions using an index.
  // The syntax is as follows: void is the return type, Chip8 represents the class, Chip8Function is the alias of the created type, and () is the input of the function
  // up up up: no method shall contain the const or static modifier so we can be able to store them all in the same table
  // we can use std::function, but it is less performant
  typedef void (Chip8::*Chip8Function)();

  /*
  The entire list of opcodes is divided into 4 categories:
The entire opcode is unique:
      $1nnn
      $2nnn
      $3xkk
      $4xkk
      $5xy0
      $6xkk
      $7xkk
      $9xy0
      $Annn
      $Bnnn
      $Cxkk
      $Dxyn
  The first digit repeats but the last digit is unique:
      $8xy0
      $8xy1
      $8xy2
      $8xy3
      $8xy4
      $8xy5
      $8xy6
      $8xy7
      $8xyE
  The first three digits are $00E but the fourth digit is unique:
      $00E0
      $00EE
  The first digit repeats but the last two digits are unique:
      $ExA1
      $Ex9E
      $Fx07
      $Fx0A
      $Fx15
      $Fx18
      $Fx1E
      $Fx29
      $Fx33
      $Fx55
      $Fx65

      this division is useful to index the opcodes inside a table
      for example the ones that are fully unique can be indexed using the 1st nibble (what a funny word) only
      so if an opcode starts with 1 then we know it is instruction 0x1nnn

      for the opcodes that are not unique we use sub-tables to further identify the intended opcode
      the 1st nibble of all opcodes goes from 1 to F, which means we will need an array of size F+1
      the size of other sub-tables is defined in the same way, for example for tableE the last opcode is 0xFx65 so the size is 0x65 + 1

    */
  Chip8Function table[0xF + 1]{};
  Chip8Function table0[0xE + 1]{};
  Chip8Function table8[0xE + 1]{};
  Chip8Function tableE[0xE + 1]{};
  Chip8Function tableF[0x65 + 1]{};

  // if a nibble starts with 0 then we need to access the table called table0
  // the task of further decoding the rest of the opcode (for example to decide whether to call $00E0 or $00EE is delegated to function Table0 which will use the last digit to call the appropriate method
  // refer to the classification of opcodes for the implementation of these methods
  void Table0();
  void Table8();
  void TableE();
  void TableF();

public:
  explicit Chip8(const std::string& filePath);
  ~Chip8();
  Keypad& getKeypad();
  void Cycle();
  [[nodiscard]] const uint32_t* getBuffer() const;
};
