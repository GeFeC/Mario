#pragma once

#include <ctime>
#include <random>
#include <vector>
#include <algorithm>
#include <tuple>
#include <glm/glm.hpp>
#include <sstream>
#include <fstream>

namespace util{
  inline static constexpr auto BigValue = 999999;

  template<typename T>
  using vector2d = std::vector<std::vector<T>>;

  inline auto rng = std::mt19937(static_cast<std::mt19937::result_type>(std::time(0)));

  inline auto random_value(int min, int max){
    auto engine = std::uniform_int_distribution{ min, max };
    return engine(rng);
  }

  struct Rect{
    glm::vec2 position;
    glm::vec2 size;

    Rect() = default;

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

  inline auto make_pair_from_vec2(const glm::vec2& vec2){
    return std::make_pair(vec2.x, vec2.y);
  }

  inline auto operator==(float value, const util::Interval& interval){
    const auto [min, max] = interval;
    return value >= min && value <= max;
  }

  inline auto operator!=(float value, const util::Interval& interval){
    return !(value == interval);
  }

  template<typename T>
  struct As{};

  template<typename T>
  As<T> as;

  template<typename Obj, typename T>
  auto operator|(const Obj& obj, const As<T>& as){
    return static_cast<T>(obj);
  }

  static auto get_file_content(const std::string& path){
    auto file = std::ifstream(path);
    auto ss = std::stringstream();
    ss << file.rdbuf();

    return ss.str();
  }
}
