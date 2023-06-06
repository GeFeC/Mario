#pragma once

#include "States/BlockState.hpp"
#include "States/MonsterState.hpp"
#include "res/textures.hpp"

#include <random>

struct FishState : MonsterState{
  glm::vec2 origin;
  float current_offset_y = 0.f;
  float max_offset = 0.f;

  enum class Type{
    Grey, Red
  } type;

private:
  inline static auto rng = std::mt19937(100);

  static auto make(const glm::vec2& position){
    auto fish = FishState();
    fish.position = position * BlockBase::Size;
    fish.origin = fish.position;
    fish.size = glm::vec2(BlockBase::Size);
    fish.walk_speed = 1.f;
    fish.reward_for_killing = 100.f;
    fish.texture_flip = Flip::UseFlip;
    fish.set_direction(DirectionLeft);
    fish.should_collide = false;
    fish.can_be_stomped = false;

    //Randomise max offset
    auto range = std::uniform_int_distribution<>(-30, 30);
    fish.max_offset = range(rng) * 2.f; 

    return fish;
  }

public:
  static auto make_grey(const glm::vec2& position){
    auto fish = make(position);
    
    fish.type = Type::Grey;
    fish.current_texture = &textures::grey_fish_swim[0];

    return fish;
  }

  static auto make_red(const glm::vec2& position){
    auto fish = make(position);
    
    fish.type = Type::Red;
    fish.walk_speed = 2.f;
    fish.set_direction(fish.direction);
    fish.current_texture = &textures::red_fish_swim[0];

    return fish;
  }
};
