#include "Memory.h"

#include <vector>

Memory::Memory(const size_t size): size(size), memory(new uint8_t[size])
{
  memset(this->memory, 0, size);
}

Memory::~Memory()
{
  delete[] this->memory;
}

void Memory::writeByte(const uint32_t address, const uint8_t data) const
{
  if (address >= this->size) throw std::out_of_range("Address out of range");
  this->memory[address] = data;
}

void Memory::writeWord(const uint32_t address, const uint16_t data) const
{
  if (address + 1 >= this->size) throw std::out_of_range("Address out of range");
  this->memory[address] = data & 0xff;
  this->memory[address + 1] = data >> 8;
}

void Memory::writeBytes(uint32_t address, const std::vector<uint8_t>& data) const
{
  if (address + data.size() >= this->size) throw std::out_of_range("Address out of range");

  for (const uint8_t byte : data)
  {
    this->memory[address++] = byte;
  }
}

uint8_t Memory::readByte(const uint32_t address) const
{
  if (address >= this->size) throw std::out_of_range("Address out of range");
  return this->memory[address];
}

uint16_t Memory::readWord(const uint32_t address) const
{
  if (address + 1 >= this->size) throw std::out_of_range("Address out of range");
  return (static_cast<uint16_t>(this->memory[address]) << 8) | this->memory[address + 1];
}

std::vector<uint8_t> Memory::readBytes(const uint32_t address, const uint32_t length) const
{
  if (address + length >= this->size) throw std::out_of_range("Address out of range");
  std::vector<uint8_t> result;
  result.reserve(length);
  for (uint32_t i = 0; i < length; i++)
  {
    result.push_back(this->memory[address + i]);
  }
  return result;
}
