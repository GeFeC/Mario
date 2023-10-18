#pragma once

namespace mario::util{

struct DirectionBase{
protected:
  int direction = 1;

public:
  constexpr explicit DirectionBase(int direction) : direction(direction) {}
  constexpr DirectionBase() : direction(1) {}

  constexpr auto as_int() const{
    return direction;
  }

  constexpr auto as_binary() const{
    return (direction + 1) / 2.f;
  }

  constexpr auto toggle(){
    direction = -direction;
  }
};

struct Direction : DirectionBase{
  constexpr explicit Direction(int direction) : DirectionBase(direction) {}

  constexpr auto operator=(const Direction&) -> Direction& = default;

  constexpr auto operator-() const{
    return Direction(-direction);
  }

  constexpr auto is_right() const{
    return ((direction + 1) / 2);
  }

  constexpr auto is_left() const{
    return !is_right();
  }

  constexpr auto is_down() const{
    return is_right();
  }

  constexpr auto is_up() const{
    return is_right();
  }

  static auto left(){
    return Direction(-1);
  }

  static auto up(){
    return Direction(-1);
  }

  static auto right(){
    return Direction(1);
  }

  static auto down(){
    return Direction(1);
  }
};

struct Flip : DirectionBase{
  constexpr explicit Flip(int direction) : DirectionBase(direction) {}

  constexpr auto operator=(const Flip&) -> Flip& = default;

  constexpr auto operator-() const{
    return Flip(-direction);
  }

  constexpr auto is_flipped() const{
    return !as_binary();
  }

  static auto flip(){
    return Flip(-1);
  }

  static auto no_flip(){
    return Flip(1);
  }
};

} //namespace mario::util
