#pragma once

#include "MonsterState.hpp"

namespace mario{

struct PlantState : virtual MonsterState{
  static constexpr auto MaxOffset = BlockBase::Size * 11 / 8;
  float offset = 0.f;
  float cooldown_duration = 0.f;
  float speed = 0.f;
  float current_cooldown = 0.f;

  enum class Direction{
    GoingUp,
    GoingDown
  } direction = Direction::GoingUp;

  enum class Type{
    Green, Red
  } type;

  auto motion_boost(){
    return size.x / BlockBase::Size;
  }

protected:

  template<typename T = PlantState>
  static auto make(const glm::vec2& position){
    auto plant = T();
    plant.position = (position + glm::vec2(0.5f, 1.f)) * BlockBase::Size;
    plant.size = glm::vec2(BlockBase::Size, BlockBase::Size * 11 / 8);
    plant.current_texture = &textures::plant[0];
    plant.can_be_stomped = false;
    plant.is_active = true;
    plant.reward_for_killing = 200;

    return plant;
  }

public:
  static auto make_green(const glm::vec2& position){
    auto plant = make(position);
    plant.type = Type::Green;

    plant.cooldown_duration = 3.f;
    plant.speed = 150.f;

    return plant;
  }

  static auto make_red(const glm::vec2& position){
    auto plant = make(position);
    plant.type = Type::Red;

    plant.cooldown_duration = 1.f;
    plant.speed = 300.f;

    return plant;
  }

};

} //namespace mario
