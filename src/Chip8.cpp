#include "Chip8.h"

#include <filesystem>
#include <fstream>
#include <iostream>

constexpr unsigned int RAM_SIZE = 4 * 1024;
constexpr unsigned int GRAPHIC_WIDTH = 64;
constexpr unsigned int GRAPHIC_HEIGHT = 32;
constexpr unsigned int STACK_SIZE = 16;
constexpr unsigned int STARTING_ADDRESS = 0x200;
constexpr unsigned int FONT_SET_START_ADDRESS = 0x50;


Chip8::Chip8():
  memory(RAM_SIZE),
  graphic(GRAPHIC_WIDTH, GRAPHIC_HEIGHT),
  programCounter("Program counter", STARTING_ADDRESS),
  index("Index register"),
  stack(STACK_SIZE),
  delayTimer("Delay timer", 0),
  soundTimer("Sound Timer", 0),
  random(0, 255)
{
  for (int i = 0; i < registers.size(); ++i)
  {
    registers[i] = Register<uint8_t>("CPU register", 0);
  }
}

Chip8::~Chip8()
= default;

void Chip8::loadRom(const char* filePath) const
{
  // set file path
  const std::filesystem::path path = filePath;

  // check if file exists
  if (!exists(path)) throw std::runtime_error("Chip8::loadRom: File does not exist");

  // The rom loads to the ram, check if (vacuous) rom is small enough to fit
  const std::uintmax_t fileSize = file_size(path);
  if (fileSize > RAM_SIZE - STARTING_ADDRESS) throw std::runtime_error("Chip8::loadRom: File too large");

  // open file
  std::ifstream file(path, std::ios::binary | std::ios::beg);

  if (!file.is_open()) throw std::runtime_error("Chip8::loadRom: Failed to open file");

  // reading rom bytes and transferring it to ram
  std::vector<uint8_t> bytes(fileSize);
  file.read(reinterpret_cast<std::istream::char_type*>(bytes.data()), static_cast<long>(fileSize));
  this->memory.writeBytes(STARTING_ADDRESS, bytes);

  file.close();
}

void Chip8::loadFont() const
{
  // 16 char at 5 bytes each
  const std::vector<uint8_t> fontSet =
  {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80 // F
  };

  this->memory.writeBytes(FONT_SET_START_ADDRESS, fontSet);
}

void Chip8::OP_00E0() const
{
  this->graphic.Clear();
}

void Chip8::OP_00EE()
{
  programCounter = stack.top();
}

void Chip8::OP_1nnn()
{
  programCounter = opcode & 0x0FFFu;
}

void Chip8::OP_2nnn()
{
  stack.push(programCounter.getAddress());
  programCounter = opcode & 0x0FFFu;
}

void Chip8::OP_3xkk()
{
  const Register<uint8_t>& Vx = registers[(opcode & 0x0F00u) >> 8u];
  const uint8_t kk = opcode & 0x00FFu;

  if (Vx == kk) programCounter.incrementBy(2);
}

void Chip8::OP_4xkk()
{
  const Register<uint8_t>& Vx = registers[(opcode & 0x0F00u) >> 8u];
  const uint8_t kk = opcode & 0x00FFu;

  if (Vx != kk) programCounter.incrementBy(2);
}

void Chip8::OP_5xy0()
{
  const Register<uint8_t>& Vx = registers[(opcode & 0x0F00u) >> 8u];
  const Register<uint8_t>& Vy = registers[(opcode & 0x00F0u) >> 4u];

  if (Vx == Vy) programCounter.incrementBy(2);
}

void Chip8::OP_6xkk()
{
  Register<uint8_t>& Vx = registers[(opcode & 0x0F00u) >> 8u];
  const uint8_t kk = opcode & 0x00FFu;

  Vx = kk;
}

void Chip8::OP_7xkk()
{
  Register<uint8_t>& Vx = registers[(opcode & 0x0F00u) >> 8u];
  const uint8_t kk = opcode & 0x00FFu;

  Vx += kk;
}

void Chip8::OP_8xy0()
{
  Register<uint8_t>& Vx = registers[(opcode & 0x0F00u) >> 8u];
  const Register<uint8_t>& Vy = registers[(opcode & 0x00F0u) >> 4u];

  Vx = Vy;
}

void Chip8::OP_8xy1()
{
  Register<uint8_t>& Vx = registers[(opcode & 0x0F00u) >> 8u];
  const Register<uint8_t>& Vy = registers[(opcode & 0x00F0u) >> 4u];

  Vx |= Vy;
}

void Chip8::OP_8xy2()
{
  Register<uint8_t>& Vx = registers[(opcode & 0x0F00u) >> 8u];
  const Register<uint8_t>& Vy = registers[(opcode & 0x00F0u) >> 4u];

  Vx &= Vy;
}

void Chip8::OP_8xy3()
{
  Register<uint8_t>& Vx = registers[(opcode & 0x0F00u) >> 8u];
  const Register<uint8_t>& Vy = registers[(opcode & 0x00F0u) >> 4u];

  Vx ^= Vy;
}

void Chip8::OP_8xy4()
{
  Register<uint8_t>& Vx = registers[(opcode & 0x0F00u) >> 8u];
  const Register<uint8_t>& Vy = registers[(opcode & 0x00F0u) >> 4u];

  const uint16_t sum = Vx.getAddress() + Vy.getAddress();

  registers[0xFu] = sum > 0xFFu;

  Vx = static_cast<uint8_t>(sum & 0x00FFu);
}

void Chip8::OP_8xy5()
{
  Register<uint8_t>& Vx = registers[(opcode & 0x0F00u) >> 8u];
  const Register<uint8_t>& Vy = registers[(opcode & 0x00F0u) >> 4u];

  registers[0xFu] = Vx > Vy;
  Vx -= Vy;
}

void Chip8::OP_8xy6()
{
  Register<uint8_t>& Vx = registers[(opcode & 0x0F00u) >> 8u];

  registers[0xFu] = Vx.getAddress() & 0x1u;

  Vx >>= 1;
}

void Chip8::OP_8xy7()
{
  Register<uint8_t>& Vx = registers[(opcode & 0x0F00u) >> 8u];
  const Register<uint8_t>& Vy = registers[(opcode & 0x00F0u) >> 4u];

  registers[0xFu] = Vy > Vx;

  Vx -= Vy;
}

void Chip8::OP_8xyE()
{
  Register<uint8_t>& Vx = registers[(opcode & 0x0F00u) >> 8u];

  registers[0xFu] = ((Vx.getAddress() & 0x80u) >> 7u);

  Vx <<= 1;
}

void Chip8::OP_9xy0()
{
  const Register<uint8_t>& Vx = registers[(opcode & 0x0F00u) >> 8u];
  const Register<uint8_t>& Vy = registers[(opcode & 0x00F0u) >> 4u];

  if (Vx != Vy) programCounter.incrementBy(2);
}

void Chip8::OP_Annn()
{
  index = opcode & 0x0FFFu;
}

void Chip8::OP_Bnnn()
{
  programCounter = (opcode & 0x0FFFu) + registers[0x0u].getAddress();
}

void Chip8::OP_Cxkk()
{
  Register<uint8_t>& Vx = registers[(opcode & 0x0F00u) >> 8u];
  const uint8_t kk = opcode & 0x00FFu;

  Vx = kk & random.generateRandomValue();
}

void Chip8::OP_Dxyn()
{
  const Register<uint8_t>& Vx = registers[(opcode & 0x0F00u) >> 8u];
  const Register<uint8_t>& Vy = registers[(opcode & 0x00F0u) >> 4u];
  const uint8_t height = opcode & 0x000Fu;

  std::vector<uint8_t> sprite = memory.readBytes(index.getAddress(), height);
  graphic.drawSprite(Vx.getAddress(), Vy.getAddress(), sprite, registers[15]);
}

void Chip8::OP_Ex9e()
{
  const Register<uint8_t>& Vx = registers[(opcode & 0x0F00u) >> 8u];

  if (keypad.isPressed(Vx.getAddress())) programCounter.incrementBy(2);
}

void Chip8::OP_ExA1()
{
  const Register<uint8_t>& Vx = registers[(opcode & 0x0F00u) >> 8u];

  if (!keypad.isPressed(Vx.getAddress())) programCounter.incrementBy(2);
}

void Chip8::OP_Fx07()
{
  Register<uint8_t>& Vx = registers[(opcode & 0x0F00u) >> 8u];

  Vx = delayTimer;
}

void Chip8::OP_Fx0A()
{
  Register<uint8_t>& Vx = registers[(opcode & 0x0F00u) >> 8u];
  while (true)
  {
    for (int i = 0; i < 16; ++i)
    {
      if (keypad.isPressed(i))
      {
        Vx = i;
        return;
      }
    }
  }
}

void Chip8::OP_Fx15()
{
  const Register<uint8_t>& Vx = registers[(opcode & 0x0F00u) >> 8u];

  delayTimer = Vx;
}

void Chip8::OP_Fx18()
{
  const Register<uint8_t>& Vx = registers[(opcode & 0x0F00u) >> 8u];

  soundTimer = Vx;
}

void Chip8::OP_Fx1E()
{
  const Register<uint8_t>& Vx = registers[(opcode & 0x0F00u) >> 8u];

  index += Vx.getAddress();
}

void Chip8::OP_Fx29()
{
  const Register<uint8_t>& Vx = registers[(opcode & 0x0F00u) >> 8u];

  // each digit sprite is 5 bytes
  index = FONT_SET_START_ADDRESS + 5 * Vx.getAddress();
}

void Chip8::OP_Fx33() const
{
  const Register<uint8_t>& Vx = registers[(opcode & 0x0F00u) >> 8u];

  memory.writeByte(index.getAddress(), Vx.getAddress() / 100);
  memory.writeByte(index.getAddress() + 1, (Vx.getAddress() / 10) % 10);
  memory.writeByte(index.getAddress() + 2, Vx.getAddress() % 10);
}

void Chip8::OP_Fx55() const
{
  const uint8_t x = (opcode & 0x0F00u) >> 8u;
  for (size_t i = 0; i < x; ++i)
  {
    memory.writeByte(index.getAddress() + i, registers[i].getAddress());
  }
}

void Chip8::OP_Fx65()
{
  const uint8_t x = (opcode & 0x0F00u) >> 8u;
  for (size_t i = 0; i < x; ++i)
  {
    registers[i] = memory.readByte(index.getAddress() + i);
  }
}
