#pragma once

#include <ctime>
#include <random>
#include <vector>
#include <algorithm>
#include <tuple>
#include <glm/glm.hpp>

namespace util{
  template<typename T>
  using vector2d = std::vector<std::vector<T>>;

  inline auto rng_once = std::mt19937(0);
  inline auto rng = std::mt19937(static_cast<std::mt19937::result_type>(std::time(0)));

  struct Rect{
    glm::vec2 position;
    glm::vec2 size;

    template<typename T>
    Rect(const T& t){
      position = t.position;
      size = t.size;
    }

    Rect(const glm::vec2& pos, const glm::vec2& size)
    : position(pos), size(size) {}
  };

  struct Interval{
    float min, max;
  };

  inline auto in_range(float min, float max){
    return Interval{ min, max };
  }

  inline auto random_value(int min, int max){
    auto engine = std::uniform_int_distribution{ min, max };
    return engine(rng);
  }

  inline auto const_random_value(int min, int max){
    auto engine = std::uniform_int_distribution{ min, max };
    return engine(rng_once);
  }

  template<typename T>
  struct Contains{
    std::vector<T> targets;
  };

  template<typename T, typename... Ts>
  inline auto contains(const T& t, const Ts&... targets){
    auto c = Contains<T>();
    c.targets = { t, targets... };
    return c;
  }

  template<typename...>
  struct TD;

  template<std::size_t N = 0, typename Callable, typename... Ts>
  inline auto tuple_for_each(std::tuple<Ts...>& tuple, const Callable& callable){
    callable(std::get<N>(tuple));

    if constexpr (N + 1 < sizeof...(Ts)){
      tuple_for_each<N + 1>(tuple, callable);
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
}

inline auto operator|(float value, const util::Interval interval){
  const auto [min, max] = interval;
  return value >= min && value <= max;
}

template<typename T>
inline auto operator|(const std::vector<T>& vec, const util::Contains<T>& target){
  return std::all_of(target.targets.begin(), target.targets.end(), [&vec = vec](auto target){
    return std::find(vec.begin(), vec.end(), target) != vec.end();
  });
}
