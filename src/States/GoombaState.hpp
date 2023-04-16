#pragma once

#include "States/MonsterState.hpp"

struct GoombaState : MonsterState{
private:
  static auto make(const glm::vec2& position){
    auto goomba = GoombaState();
    goomba.position = position * BlockBase::Size;
    goomba.size = glm::vec2(BlockBase::Size);
    goomba.walk_speed = 3.f;

    return goomba;
  }

public:
  static auto make_normal(const glm::vec2& position, Direction direction = DirectionLeft){
    auto goomba = make(position);
    goomba.set_direction(direction);
    goomba.current_texture = &textures::goomba_walk[0];
    goomba.reward_for_killing = 100;

    return goomba;
  }

  static auto make_red(const glm::vec2& position, Direction direction = DirectionLeft){
    auto goomba = make(position);
    goomba.set_direction(direction);
    goomba.current_texture = &textures::red_goomba_walk[0];
    goomba.fall_from_edge = false;
    goomba.is_active = true;
    goomba.reward_for_killing = 100;

    return goomba;
  }

  static auto make_yellow(const glm::vec2& position, Direction direction = DirectionLeft){
    auto goomba = make(position);

    goomba.walk_speed = 8.f;
    goomba.set_direction(direction);
    goomba.current_texture = &textures::yellow_goomba_walk[0];
    goomba.fall_from_edge = false;
    goomba.is_active = true;
    goomba.reward_for_killing = 300;

    return goomba;
  }
};
