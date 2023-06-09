#pragma once

#include "States/MonsterState.hpp"

namespace mario{

struct MushroomState : MonsterState{
  enum class Type{
    Red, Green
  } type = Type::Red;

  float offset = 0.f;

  auto disappear(){
    is_active = false;
    is_visible = false;
    position.y = util::BigValue;
  }

private:
  static auto make(const glm::vec2& position, Direction direction = DirectionLeft){
    auto mushroom = MushroomState();
    mushroom.position = position * BlockBase::Size;
    mushroom.size = glm::vec2(BlockBase::Size);
    mushroom.walk_speed = 5.f;
    mushroom.set_direction(direction);
    mushroom.is_visible = false;
    mushroom.reward_for_killing = 500.f;

    return mushroom;
  }

public:
  static auto make_red(const glm::vec2& position, Direction direction = DirectionLeft){
    auto mushroom = make(position, direction);
    mushroom.current_texture = &textures::mushroom;
    mushroom.type = Type::Red;

    return mushroom;
  }

  static auto make_green(const glm::vec2& position, Direction direction = DirectionLeft){
    auto mushroom = make(position, direction);
    mushroom.current_texture = &textures::green_mushroom;
    mushroom.type = Type::Green;

    return mushroom;
  }
};

} //namespace mario
