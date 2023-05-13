#pragma once
#include <tuple>
#include <vector>
#include <type_traits>

namespace util::poly{

template<typename... Ts>
struct Array : std::tuple<std::vector<Ts>...>{
  template<typename T>
  auto& get_vec(){
    return std::get<std::vector<T>>(*this);
  }

  template<typename T>
  auto& get(std::size_t index){
    return get_vec<T>()[index];
  }

  template<typename T>
  auto& push(const T& value){
    get_vec<T>().push_back(value);
    return get_vec<T>().back();
  }

  template<std::size_t N = 0, typename Function>
  auto for_each(const Function& function){
    function(std::get<N>(*this));

    if constexpr (N + 1 < sizeof...(Ts)){
      for_each<N + 1>(function);
    }
  }

  template<template<typename> typename Controller, typename... Deps>
  auto run_controllers(Deps&... deps){
    auto deps_tuple = std::make_tuple(std::ref(deps)...);
    
    for_each([&](auto& vec){
      for (auto& object : vec) {
        using object_type = std::decay_t<decltype(object)>;

        using fun_params_t = typename FunctionInfo<decltype(Controller<object_type>::run)>::params_types;
        fun_params_t fun_params;

        std::apply([&](auto& first_param, auto&... params){
          Controller<object_type>::run(
            object,
            std::get<std::decay_t<decltype(params)>&>(deps_tuple)...
          );
        }, fun_params);
      }
    });
  }
};

} //namespace util::poly
