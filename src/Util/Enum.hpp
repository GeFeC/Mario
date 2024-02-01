#pragma once

#include "Util/Cast.hpp"
#include "Util/Interval.hpp"

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

template<typename Enum>
static auto is_enum_between(Enum target, Enum min, Enum max){
	return (target | util::as<int>) == util::in_range(
		min | util::as<int>,
		max | util::as<int>
	);
}

} //namespace mario::util
