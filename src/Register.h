#pragma once
#include <string>

template <typename T>
class Register
{
  // useful in debugging
  std::string registerName;

  T address;

public:
  // explicit Register() = default;
  explicit Register(std::string registerName, T address): registerName(std::move(registerName)), address(address)
  {
  }

  void setAddress(T value)
  {
    address = value;
  }

  T getAddress() const
  {
    return address;
  }

  void increment()
  {
    ++address;
  }

  void incrementBy(T size)
  {
    address += size;
  }

  void decrement()
  {
    --address;
  }

  void decrementBy(T size)
  {
    address -= size;
  }

  Register& operator=(const T value)
  {
    address = value;

    return *this;
  }

  Register& operator=(const Register& value)
  {
    if (this == &value) return *this;
    address = value.getAddress();

    return *this;
  }

  Register& operator+=(const T& value)
  {
    address += value;

    return *this;
  }

  Register& operator+=(const Register& value)
  {
    address += value.getAddress();

    return *this;
  }

  Register& operator-=(const T& value)
  {
    address -= value;

    return *this;
  }

  Register& operator-=(const Register& value)
  {
    address -= value.getAddress();

    return *this;
  }

  Register& operator/=(const T& value)
  {
    address -= value;

    return *this;
  }

  Register& operator/=(const Register& value)
  {
    address -= value.getAddress();

    return *this;
  }

  Register& operator|=(const T& value)
  {
    address |= value;

    return *this;
  }

  Register& operator|=(const Register& value)
  {
    address |= value.getAddress();

    return *this;
  }

  Register& operator&=(const T& value)
  {
    address &= value;

    return *this;
  }

  Register& operator&=(const Register& value)
  {
    address &= value.getAddress();

    return *this;
  }

  Register& operator^=(const T& value)
  {
    address ^= value;

    return *this;
  }

  Register& operator^=(const Register& value)
  {
    address ^= value.getAddress();

    return *this;
  }

  Register& operator>>=(const uint shift)
  {
    address >>= shift;

    return *this;
  }

  Register& operator<<=(const uint shift)
  {
    address <<= shift;

    return *this;
  }

  bool operator==(const T& value) const
  {
    return address == value;
  }

  bool operator==(const Register& value) const
  {
    return address == value.getAddress();
  }


  bool operator!=(const T& value) const
  {
    return address != value;
  }

  bool operator!=(const Register& value) const
  {
    return address != value.getAddress();
  }


  bool operator>(const T& value) const
  {
    return address > value;
  }

  bool operator>(const Register& value) const
  {
    return address > value.getAddress();
  }

  bool operator<(const T& value) const
  {
    return address < value;
  }

  bool operator<(const Register& value) const
  {
    return address < value.getAddress();
  }

  bool operator>=(const T& value) const
  {
    return address >= value;
  }

  bool operator>=(const Register& value) const
  {
    return address >= value.getAddress();
  }

  bool operator<=(const T& value) const
  {
    return address <= value;
  }

  bool operator<=(const Register& value) const
  {
    return address <= value.getAddress();
  }
};
