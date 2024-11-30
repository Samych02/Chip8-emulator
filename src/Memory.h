#pragma once

#include <cstdint>

template <typename T>
class Memory
{
  size_t size;
  T* memory;

public:
  explicit Memory(const size_t size) : size(size), memory(new T[size])
  {
    memset(memory, 0, sizeof(T) * size);
  }

  ~Memory()
  {
    delete[] memory;
  }

  void writeByte(size_t address, const uint8_t data) const
  {
    if (address >= size) throw std::out_of_range("Address out of range");
    memory[address] = data;
  }

  void writeWord(size_t address, const uint16_t data) const
  {
    if (address + 1 >= size) throw std::out_of_range("Address out of range");
    memory[address] = static_cast<uint8_t>(data & 0xff);
    memory[address + 1] = static_cast<uint8_t>(data >> 8);
  }

  void writeBytes(size_t address, const std::vector<uint8_t>& data) const
  {
    if (address + data.size() >= size) throw std::out_of_range("Address out of range");

    for (const uint8_t byte : data)
    {
      memory[address++] = byte;
    }

  }
  [[nodiscard]] uint8_t readByte(const size_t address) const
  {
    if (address >= size) throw std::out_of_range("Address out of range");
    return memory[address];
  }

  [[nodiscard]] uint16_t readWord(const size_t address) const
  {
    if (address + 1 >= size) throw std::out_of_range("Address out of range");
    return (static_cast<uint16_t>(memory[address]) << 8) | memory[address + 1];
  }

  std::vector<T> readBytes(const size_t startingAddress, const size_t length) const
  {
    if (startingAddress + length >= size) throw std::out_of_range("Address out of range");
    std::vector<T> result;
    result.reserve(length);
    for (size_t i = 0; i < length; i++)
    {
      result.push_back(memory[startingAddress + i]);
    }
    return result;
  }};
