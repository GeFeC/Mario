#pragma once

#include "States/BlockState.hpp"
#include "States/MonsterState.hpp"
#include "res/textures.hpp"

#include <random>

namespace mario{

struct FishState : MonsterState{
  glm::vec2 origin;
  float current_offset_y = 0.f;
  float max_offset = 0.f;

  bool started_jumping = false;

  enum class Type{
    Grey, Red, Flame
  } type;

  enum class MotionType{
    Swimming, Jumping
  } motion_type = MotionType::Swimming;

  enum class JumpDirection{
    Left, Right
  } jump_direction = JumpDirection::Left;

  static auto random_value(int min, int max){
    auto range = std::uniform_int_distribution<>(min, max);
    return range(rng);
  }

private:
  inline static auto rng = std::mt19937(100);

  static auto make(const glm::vec2& position){
    auto fish = FishState();
    fish.position = position * BlockBase::Size;
    fish.origin = fish.position;
    fish.size = glm::vec2(BlockBase::Size);
    fish.walk_speed = 1.f;
    fish.reward_for_killing = 100.f;
    fish.texture_flip = util::Flip::flip();
    fish.should_collide = false;
    fish.can_be_stomped = false;

    //Randomise max offset
    fish.max_offset = random_value(-30, 30) * 2.f;

    return fish;
  }

public:
  static auto make_grey(const glm::vec2& position){
    auto fish = make(position);

    fish.set_direction(util::Direction::left());
    fish.type = Type::Grey;
    fish.current_texture = &textures::grey_fish_swim[0];

    return fish;
  }

  static auto make_red(const glm::vec2& position){
    auto fish = make(position);
    
    fish.type = Type::Red;
    fish.walk_speed = 2.f;
    fish.set_direction(util::Direction::left());
    fish.current_texture = &textures::red_fish_swim[0];

    return fish;
  }

  static auto make_jumping(const glm::vec2& position){
    auto fish = make_red(position);

    fish.motion_type = FishState::MotionType::Jumping;
    fish.jump_direction = FishState::random_value(0, 1) | util::as<FishState::JumpDirection>;
    fish.can_be_stomped = true;

    return fish;
  }
};

} //namespace mario
