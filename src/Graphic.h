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
    memset(buffer, 0, sizeof(T) * size);
  }

  ~Graphic()
  {
    delete[] buffer;
  }

  void Clear() const
  {
    memset(buffer, 0, sizeof(T) * size);
  }

  [[nodiscard]] size_t GetWidth() const
  {
    return width;
  }

  [[nodiscard]] size_t GetHeight() const
  {
    return height;
  }

  // if I understood correctly, the drawing mechanism is as follows:
  // We read n byte from memory starting at location I
  // The number n actually represents the height of the sprite we want to draw.
  // each byte represent a row of 8 pixels (bits) which will be put in video buffer
  // the next byte from memory will be put in the next row.
  // then we set Vf to 1 for collision detection
  // edit: we are using uint32 to make compatible with SDL_UpdateTexture so 0x0 for 0 and 0xFFFFFFFF for 1
  void drawSprite(const size_t Vx, const size_t Vy, const std::vector<uint8_t>& sprite, Register<uint8_t>& VF)
  {
    const size_t posX = Vx % width;
    const size_t posY = Vy % height;

    for (int spriteRowIndex = 0; spriteRowIndex < sprite.size(); ++spriteRowIndex)
    {
      const uint8_t spriteRow = sprite[spriteRowIndex];
      for (int pixelIndex = 0; pixelIndex < 8; ++pixelIndex)
      {
        // we get the pixel from buffer at coordinates (posX, posY)
        // that formule convert coordinates (x,y) to an array index
        // the % to wrap to the other side
        T& bufferPixel = buffer[(((posY + spriteRowIndex) * width) ) + (posX + pixelIndex)];
        // for optimization purposes, we are going to skip the off pixel in the sprite
        if (const uint8_t& spritePixel = (spriteRow >> (7 - pixelIndex)) & 0x1u; spritePixel == 0x1u)
        {
          if (bufferPixel == 0xFFFFFFFFu)
          {
            VF.setAddress(1);
          }
          bufferPixel ^= 0xFFFFFFFFu;
        }
      }
    }
  }

  T* getBuffer() const
  {
    return buffer;
  }
};
