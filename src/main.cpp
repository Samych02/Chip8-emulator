#include <iostream>

#include "RandomGenerator.h"

int main()
{
  RandomGenerator<uint8_t> g(0,255U);
  std::cout << static_cast<int>(g.generateRandomValue()) << std::endl;

  return 0;
}
