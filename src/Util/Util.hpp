#pragma once

#include <type_traits>
#include <vector>

namespace mario::util{

//Constants:
inline static constexpr auto BigValue = 999999;

//Aliases:
template<typename T, typename Base>
using not_derived = std::enable_if_t<(!std::is_base_of_v<Base, T> && !std::is_same_v<T, Base>)>;

template<typename T>
using vector2d = std::vector<std::vector<T>>;

} //namespace mario::util
