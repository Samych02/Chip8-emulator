#pragma once

#include <array>
#include <cstdint>

template <typename T>
class Memory
{
  size_t size;
  T* memory;

public:
  explicit Memory(const size_t size) : size(size), memory(new T[size])
  {
    memset(this->memory, 0, sizeof(T) * size);
  }

  ~Memory()
  {
    delete[] memory;
  }

  void writeByte(size_t address, uint8_t data) const
  {
    if (address >= this->size) throw std::out_of_range("Address out of range");
    memory[address] = data;
  }

  void writeWord(size_t address, const uint16_t data) const
  {
    if (address + 1 >= this->size) throw std::out_of_range("Address out of range");
    this->memory[address] = data & 0xff;
    this->memory[address + 1] = data >> 8;
  }

  void writeBytes(size_t address, const std::vector<T>& data) const
  {
    if (address + data.size() >= this->size) throw std::out_of_range("Address out of range");

    for (const T byte : data)
    {
      this->memory[address++] = byte;
    }

  }
  [[nodiscard]] uint8_t readByte(const size_t address) const
  {
    if (address >= this->size) throw std::out_of_range("Address out of range");
    return this->memory[address];
  }

  [[nodiscard]] uint16_t readWord(const size_t address) const
  {
    if (address + 1 >= this->size) throw std::out_of_range("Address out of range");
    return (static_cast<uint16_t>(this->memory[address]) << 8) | this->memory[address + 1];
  }

  std::vector<T> readBytes(const size_t startingAddress, const size_t length) const
  {
    if (startingAddress + length >= this->size) throw std::out_of_range("Address out of range");
    std::vector<T> result;
    result.reserve(length);
    for (size_t i = 0; i < length; i++)
    {
      result.push_back(this->memory[startingAddress + i]);
    }
    return result;
  }};
