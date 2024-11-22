#pragma once
#include <string>

template <typename T>
class Register
{
  // useful in debugging
  std::string registerName;

  T address;

public:
  explicit Register(std::string registerName) : registerName(std::move(registerName)), address(0)
  {
  }

  explicit Register(std::string registerName, T address): registerName(std::move(registerName)), address(address)
  {
  }

  void setAddress(T address)
  {
    this->address = address;
  }

  T getAddress() const
  {
    return this->address;
  }

  void increment()
  {
    ++this->address;
  }

  void incrementBy(T size)
  {
    this->address += size;
  }

  void decrement()
  {
    --this->address;
  }

  void decrementBy(T size)
  {
    this->address -= size;
  }

  Register& operator=(const T value)
  {
    this->address == value;

    return *this;
  }

  Register& operator=(const Register& value)
  {
    if (this == &value) return *this;
    this->address == value.getAddress();

    return *this;
  }

  Register& operator-=(const T& value)
  {
    this->address -= value;

    return *this;
  }

  Register& operator-=(const Register& value)
  {
    this->address -= value.getAddress();

    return *this;
  }

  Register& operator/=(const T& value)
  {
    this->address -= value;

    return *this;
  }

  Register& operator/=(const Register& value)
  {
    this->address -= value.getAddress();

    return *this;
  }

  Register& operator|=(const T& value)
  {
    this->address |= value;

    return *this;
  }

  Register& operator|=(const Register& value)
  {
    this->address |= value.getAddress();

    return *this;
  }

  Register& operator&=(const T& value)
  {
    this->address &= value;

    return *this;
  }

  Register& operator&=(const Register& value)
  {
    this->address &= value.getAddress();

    return *this;
  }

  Register& operator^=(const T& value)
  {
    this->address &= value;

    return *this;
  }

  Register& operator^=(const Register& value)
  {
    this->address &= value.getAddress();

    return *this;
  }

  Register& operator>>=(const uint shift)
  {
    this->address >>= shift;

    return *this;
  }

  Register& operator<<=(const uint shift)
  {
    this->address <<= shift;

    return *this;
  }

  bool operator==(const T& value) const
  {
    return this->address == value;
  }

  bool operator==(const Register& value) const
  {
    return this->address == value.getAddress();
  }


  bool operator!=(const T& value) const
  {
    return this->address != value;
  }

  bool operator!=(const Register& value) const
  {
    return this->address != value.getAddress();
  }


  bool operator>(const T& value) const
  {
    return this->address > value;
  }

  bool operator>(const Register& value) const
  {
    return this->address > value.getAddress();
  }

  bool operator<(const T& value) const
  {
    return this->address < value;
  }

  bool operator<(const Register& value) const
  {
    return this->address < value.getAddress();
  }

  bool operator>=(const T& value) const
  {
    return this->address >= value;
  }

  bool operator>=(const Register& value) const
  {
    return this->address >= value.getAddress();
  }

  bool operator<=(const T& value) const
  {
    return this->address <= value;
  }

  bool operator<=(const Register& value) const
  {
    return this->address <= value.getAddress();
  }
};
