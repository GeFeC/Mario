#pragma once

namespace mario::util{

struct Interval{
  float min, max;
};

inline auto in_range(float min, float max){
  return Interval{ min, max };
}

inline auto operator==(float value, const Interval& interval){
  const auto [min, max] = interval;
  return value >= min && value <= max;
}

inline auto operator!=(float value, const Interval& interval){
  return !(value == interval);
}

} //namespace mario::util
