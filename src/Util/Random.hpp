#pragma once

#include <random>
#include <ctime>

namespace mario::util{

//Randomising:
inline auto rng = std::mt19937(static_cast<std::mt19937::result_type>(std::time(0)));

inline auto random_value(int min, int max){
  auto engine = std::uniform_int_distribution{ min, max };
  return engine(rng);
}

} //namespace mario::util
