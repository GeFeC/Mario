#pragma once

template<typename T>
struct Controller{
  static auto run(T& t){}
};

template<typename T>
struct BackgroundView{
  static auto run(const T& t){}
};

template<typename T>
struct PlantsView{
  static auto run(const T& t){}
};

template<typename T>
struct BlocksView{
  static auto run(const T& t){}
};

template<typename T>
struct EntitiesView{
  static auto run(const T& t){}
};
