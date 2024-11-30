#pragma once
#include <cstdint>

#include "Register.h"

template <typename T>
class Stack
{
  size_t size;
  T* stack;
  // hardcoding the type of sp because the stack is represented as an array, so we only need the index
  Register<uint8_t> stackPointer;

public:
  explicit Stack(const size_t size): size(size), stack(new T[size]), stackPointer("Stack pointer" , 0x0u)
  {
  }

  ~Stack()
  {
    delete[] stack;
  }

  [[nodiscard]] bool isEmpty() const
  {
    return stackPointer.getAddress() == 0;
  }

  [[nodiscard]] bool isFull() const
  {
    return stackPointer.getAddress() == size;
  }

  void push(const T value)
  {
    if (isFull()) throw std::out_of_range("Stack overflow");
    stack[stackPointer.getAddress()] = value;
    stackPointer.increment();
  }

  T pop()
  {
    if (isEmpty()) throw std::out_of_range("Stack underflow");
    stackPointer.decrement();
    return stack[stackPointer.getAddress()];
  }

  T top()
  {
    if (isEmpty()) throw std::out_of_range("Empty stack");
    return stack[stackPointer.getAddress()];
  }

  [[nodiscard]] size_t getCapacity() const
  {
    return size - stackPointer.getAddress();
  }

  // [[nodiscard]] T get(const size_t index) const
  // {
  //   if (isEmpty()) throw std::out_of_range("Empty stack");
  //   if (index >= stackPointer.getAddress()) throw std::out_of_range("Invalid index");
  //
  //   return stack[index];
  // }
};
