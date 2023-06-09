#pragma once

#include "Util/Util.hpp"

#include <tuple>
#include <vector>
#include <type_traits>

namespace mario::util::poly{

template<typename T>
struct FunctionInfo{};

template<typename Ret, typename... Params>
struct FunctionInfo<Ret(Params...)>{
  using return_type = Ret;
  using params_types = std::tuple<std::decay_t<Params>...>;

  //When object is needed to be created, but some types are not default constructible:
  using params_ptr_types = std::tuple<std::decay_t<Params>*...>;

  static constexpr auto params_count = sizeof...(Params);
};

template<template <typename> typename Template, typename Function>
struct ForEachTemplate{
  Function function;

  template<typename T>
  auto invoke(Template<T>& object){
    function(object);
  }

  template<typename T>
  auto invoke(const Template<T>& object){
    function(object);
  }

  template<typename T>
  auto invoke(const T& object){ /* empty */ }
};

template<typename... Ts>
struct Array : std::tuple<std::vector<Ts>...>{
  using this_type = Array<Ts...>;

  template<typename T>
  static constexpr auto ContainsType = std::disjunction<std::is_same<T, Ts>...>::value;

  template<typename T>
  auto& back(){
    return get_vec<T>().back();
  }

  template<typename T>
  auto size(){
    return get_vec<T>().size();
  }

  template<typename T>
  auto& get_vec(){
    return std::get<std::vector<T>>(*this);
  }

  template<typename T>
  const auto& get_vec() const{
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
    auto& vec = std::get<N>(*this);
    for (auto& item : vec){
      function(item);
    }

    if constexpr (N + 1 < sizeof...(Ts)){
      for_each<N + 1>(function);
    }
  }

  template<typename Function>
  auto for_each(const Function& function) const{
    const_cast<this_type*>(this)->for_each([&](const auto& e){ function(e); });
  }

  template<typename... Types, typename Function>
  auto for_each_type(const Function& function){
    for_each([&](auto& item){
      if constexpr (std::disjunction<std::is_same<Types, std::decay_t<decltype(item)>>...>::value){
        function(item);
      }
    });
  }

  template<typename... Types, typename Function>
  auto for_each_type(const Function& function) const{
    const_cast<this_type*>(this)->for_each_type<Types...>([&](const auto& e){ function(e); });
  }

  template<template <typename> typename Template, typename Function>
  auto for_each_template(const Function& function){
    ForEachTemplate<Template, Function> invoker{ function };

    for_each([&](auto& object){
      invoker.invoke(object);
    });
  }

  template<template <typename> typename Template, typename Function>
  auto for_each_template(const Function& function) const{
    const_cast<this_type*>(this)->for_each_template<Template>([&](const auto& e){ function(e); });
  }

  template<typename T, typename Function>
  auto for_each_derived(const Function& function){
    for_each([&](auto& object){
      using ObjectType = std::decay_t<decltype(object)>;

      if constexpr (std::is_base_of_v<T, ObjectType>){
        function(object);
      }
    });
  }

  template<typename T, typename Function>
  auto for_each_derived(const Function& function) const{
    const_cast<this_type*>(this)->for_each_derived<T>([&](const auto& e){ function(e); });
  }

  template<bool IsConst, template<typename> typename Controller, typename... Deps>
  auto run_controllers_base(Deps&... deps){
    auto deps_tuple = std::make_tuple(std::ref(deps)...);
    
    for_each([&](auto& object){
      using object_type = std::decay_t<decltype(object)>;
      using fun_info = FunctionInfo<decltype(Controller<object_type>::run)>;
      using fun_param_ptrs_t = typename fun_info::params_ptr_types;
      const auto params = fun_param_ptrs_t{};

      std::apply([&](auto, auto... params){
        if constexpr (!IsConst)
          Controller<object_type>::run(
            object,
            std::get<std::remove_pointer_t<decltype(params)>&>(deps_tuple)...
          );

        if constexpr (IsConst)
          Controller<object_type>::run(
            object,
            std::get<const std::remove_pointer_t<decltype(params)>&>(deps_tuple)...
          );
      }, params);
    });
  }

  template<template<typename> typename Controller, typename... Deps>
  auto run_controllers(Deps&... deps) const{
    const_cast<this_type*>(this)->run_controllers_base<true, Controller, Deps...>(deps...);
  }

  template<template<typename> typename Controller, typename... Deps>
  auto run_controllers(Deps&... deps) {
    run_controllers_base<false, Controller, Deps...>(deps...);
  }

};

} //namespace mario::util::poly
