#pragma once
#include <random>

template <typename T>
class RandomGenerator
{
  std::default_random_engine generator;
  std::uniform_int_distribution<T> distribution;

public:
  RandomGenerator(T min, T max): generator(std::chrono::system_clock::now().time_since_epoch().count()),
                                 distribution(std::uniform_int_distribution<uint8_t>(min, max))
  {
  }

  T generateRandomValue()
  {
    return distribution(generator);
  }
};
