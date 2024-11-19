#pragma once
#include <array>
#include <cstdint>

class Memory
{
  size_t size;
  uint8_t* memory;

public:
  explicit Memory(size_t size);
  ~Memory();
  void writeByte(uint32_t address, uint8_t data) const;
  void writeWord(uint32_t address, uint16_t data) const;
  void writeBytes(uint32_t address, const std::vector<uint8_t>& data) const;
  [[nodiscard]] uint8_t readByte(uint32_t address) const;
  [[nodiscard]] uint16_t readWord(uint32_t address) const;
  [[nodiscard]] std::vector<uint8_t> readBytes(uint32_t address, uint32_t length) const;
};
