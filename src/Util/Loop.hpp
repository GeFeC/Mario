#pragma once

#include <tuple>
#include <functional>

namespace mario::util{

template<std::size_t N = 0, typename Callable, typename... Ts>
inline auto tuple_for_each(std::tuple<Ts...>& tuple, const Callable& callable){
  callable(std::get<N>(tuple));

  if constexpr (N + 1 < sizeof...(Ts)){
    tuple_for_each<N + 1>(tuple, callable);
  }
}

template<typename Callable, typename T>
inline auto for_2d(const Callable& callable, const std::pair<T, T>& min, const std::pair<T, T>& max){
  const auto [x_min, y_min] = min;
  const auto [x_max, y_max] = max;

  for (int x = x_min; x < x_max; ++x){
    for (int y = y_min; y < y_max; ++y){
      callable(x, y);
    }
  }
}

template<typename Callable, typename... Containers>
inline auto multi_for(const Callable& callable, Containers&... containers){
  auto tuple = std::make_tuple(std::ref(containers)...);

  tuple_for_each(tuple, [&callable](auto& container){
    for (auto& item : container){
      callable(item);
    }
  });
}

template<typename Callable, typename T>
inline auto for_2d(const Callable& callable, const std::pair<T, T>& max){
  for_2d<Callable, T>(
      callable, 
      std::make_pair(0, 0),
      max
  );
}

} //namespace mario::util
