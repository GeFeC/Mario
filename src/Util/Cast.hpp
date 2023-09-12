#pragma once

namespace mario::util{

template<typename T>
struct As{};

template<typename T>
As<T> as;

template<typename Obj, typename T>
decltype(auto) operator|(Obj&& obj, const As<T>& as){
  return static_cast<T>(obj);
}

} //namespace mario::util
