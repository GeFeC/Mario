#pragma once

#include "Controllers/PointsParticlesController.hpp"
#include "Controllers/EntityController.hpp"
#include "PolyControllers.hpp"
#include "States/LevelState.hpp"

#include "Controllers/MonsterController.hpp"
#include "res/textures.hpp"

namespace mario::fish_controller{

static auto controller_base(FishState& fish, LevelState& level){
  monster_controller::become_active_when_seen(fish, level);
  entity_controller::movement(fish, level);
  entity_controller::kill_player_on_touch(fish, level.player);
  monster_controller::die_when_hit_by_fireball(fish, level);
  monster_controller::points_particles(fish);

  if (fish.was_hit) {
    entity_controller::gravity(fish, level);
    return;
  }

  const auto offset = glm::sin(glfwGetTime()) * fish.max_offset;
  fish.position.y = fish.origin.y + offset;
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
