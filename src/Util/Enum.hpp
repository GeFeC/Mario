#pragma once

#include "Util/Cast.hpp"

namespace mario::util{

template<typename Enum>
static auto enum_add(Enum enum_value, float value){
  const auto int_value = enum_value | as<int>;
  return (int_value + value) | as<Enum>;
}

template<typename Enum>
static auto enum_multiply(Enum enum_value, float value){
  const auto int_value = enum_value | as<int>;
  return (int_value * value) | as<Enum>;
}

template<typename Enum, typename Function>
static auto enum_modify(Enum enum_value, const Function& function){
  return function(enum_value | as<int>);
}

} //namespace mario::util
