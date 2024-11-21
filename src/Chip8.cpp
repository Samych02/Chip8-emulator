#include "Chip8.h"

#include <filesystem>
#include <fstream>
#include <iostream>

constexpr unsigned int RAM_SIZE = 4 * 1024;
constexpr unsigned int GRAPHIC_SIZE = 32 * 64;
constexpr unsigned int STACK_SIZE = 16;
constexpr unsigned int STARTING_ADDRESS = 0x200;
constexpr unsigned int FONT_SET_START_ADDRESS = 0x50;


Chip8::Chip8(): memory(RAM_SIZE),
                graphic(GRAPHIC_SIZE),
                programCounter("Program counter", STARTING_ADDRESS),
                stack(STACK_SIZE),
                random(0, 255)
{
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
  programCounter.setAddress(stack.top());
}

void Chip8::OP_1nnn()
{
  programCounter.setAddress(opcode & 0x0FFFu);
}

void Chip8::OP_2nnn()
{
  stack.push(programCounter.getAddress());
  programCounter.setAddress(opcode & 0x0FFFu);
}



