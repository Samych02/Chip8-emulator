#pragma once
#include <cstdint>
#include <cstddef>

template <typename T>
class Graphic
{
  size_t size;
  T* buffer;

public:
  explicit Graphic(const size_t size) : size(size), buffer(new T[size])
  {
    memset(this->buffer, 0, sizeof(T) * size);
  }

  ~Graphic()
  {
    delete[] this->buffer;
  }

  void Clear() const
  {
    memset(this->buffer, 0, sizeof(T) * size);
  }
};
