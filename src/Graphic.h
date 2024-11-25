#pragma once
#include <cstddef>
#include <cstring>
#include <vector>

#include "Register.h"

template <typename T>
class Graphic
{
  size_t width;
  size_t height;
  size_t size;
  T* buffer;

public:
  explicit Graphic(const size_t width, const size_t height) : width(width), height(height), size(width * height),
                                                              buffer(new T[size])
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

  [[nodiscard]] size_t GetWidth() const
  {
    return this->width;
  }

  [[nodiscard]] size_t GetHeight() const
  {
    return this->height;
  }

  // if I understood correctly, the drawing mechanism is as follows:
  // We read n byte from memory starting at location I
  // The number n actually represents the height of the sprite we want to draw.
  // each byte represent a row of 8 pixels (bits) which will be put in video buffer
  // the next byte from memory will be put in the next row.
  // then we set Vf to 1 for collision detection
  void drawSprite(size_t Vx, size_t Vy, std::vector<T>& sprite, Register<uint8_t> VF)
  {
    size_t posX = Vx % this->width;
    size_t posY = Vy % this->height;

    for (int spriteRowIndex = 0; spriteRowIndex < sprite.size(); ++spriteRowIndex)
    {
      T& spriteRow = sprite[spriteRowIndex];
      for (int pixelIndex = 0; pixelIndex < 8; ++pixelIndex)
      {
        // we get the pixel from buffer at coordinates (posX, posY)
        // that formule convert coordinates (x,y) to an array index
        // the % to wrap to the other side
        T& bufferPixel = buffer[(((posY + spriteRowIndex) * this->width) % this->height) + ((posX + pixelIndex) % this->width)];
        T& spritePixel = sprite[(spriteRow >> (7 - spriteRowIndex)) & 0x1u];

        // for optimization purposes, we are going to skip the off pixel in the sprite
        if (spritePixel == 0x1u)
        {
        if (bufferPixel == 0x1u) VF.setAddress(1);
          bufferPixel ^= 0x1u;
        }
      }
    }
  }
};
