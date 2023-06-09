#pragma once

#include "Controllers/PointsParticlesController.hpp"
#include "Controllers/EntityController.hpp"
#include "PolyControllers.hpp"
#include "States/BlockState.hpp"
#include "States/LevelState.hpp"

#include "Controllers/MonsterController.hpp"
#include "res/textures.hpp"

namespace mario::fish_controller{

static auto swimming_fish_motion(FishState& fish){
  const auto offset = glm::sin(glfwGetTime()) * fish.max_offset;
  fish.position.y = fish.origin.y + offset;
}

static auto jumping_fish_motion(FishState& fish, const LevelState& level){
  if (!fish.started_jumping){
    const auto distance_to_player = fish.position.x - level.player.position.x;
    const auto fish_see_distance = FishState::random_value(5, 10);

    const auto fish_see_player_on_left 
      = distance_to_player < BlockBase::Size * fish_see_distance && fish.jump_direction == FishState::JumpDirection::Left;

    const auto fish_see_player_on_right
      = distance_to_player < -BlockBase::Size * fish_see_distance && fish.jump_direction == FishState::JumpDirection::Right;

    const auto fish_see_player = fish_see_player_on_right || fish_see_player_on_left;

    if (fish_see_player){
      fish.started_jumping = true;
      fish.walk_speed = glm::abs(distance_to_player) / BlockBase::Size / 1.5f;
      fish.gravity = FishState::random_value(-32, -25);
      fish.gravity_boost = 0.5f;
    }

    if (fish_see_player_on_left) fish.set_direction(EntityState::DirectionLeft);
    else if (fish_see_player_on_right) fish.set_direction(EntityState::DirectionRight);
  }
  else{
    entity_controller::gravity(fish, level);
    entity_controller::movement(fish, level);
  }
}

static auto controller_base(FishState& fish, LevelState& level){
  if (fish.motion_type == FishState::MotionType::Swimming){
    entity_controller::movement(fish, level);
  }
  else{
    monster_controller::die_when_stomped(fish, level, [&]{
      monster_controller::bounce_out(fish);
      fish.gravity_boost = 1.f;
    });
  }

  entity_controller::kill_player_on_touch(fish, level.player);
  monster_controller::become_active_when_seen(fish, level);
  monster_controller::die_when_hit_by_fireball(fish, level);
  monster_controller::points_particles(fish);

  if (fish.was_hit) {
    entity_controller::gravity(fish, level);
    return;
  }

  switch(fish.motion_type){
    case FishState::MotionType::Swimming: swimming_fish_motion(fish); break;
    case FishState::MotionType::Jumping: jumping_fish_motion(fish, level); break;
  }
}

static auto grey_fish_controller(FishState& fish, LevelState& level){
  controller_base(fish, level);
  monster_controller::run_movement_animation(fish, textures::grey_fish_swim);
}

static auto red_fish_controller(FishState& fish, LevelState& level){
  controller_base(fish, level);
  monster_controller::run_movement_animation(fish, textures::red_fish_swim);
}

} //namespace mario::fish_controller

namespace mario{

template<>
struct Controller<FishState>{
  static auto run(FishState& fish, LevelState& level){
    switch(fish.type){
      case FishState::Type::Grey: fish_controller::grey_fish_controller(fish, level); break;
      case FishState::Type::Red: fish_controller::red_fish_controller(fish, level); break;
    }
  }
};

} //namespace mario
