#pragma once

#include "Controllers/PointsParticlesController.hpp"
#include "Controllers/EntityController.hpp"
#include "PolyControllers.hpp"
#include "States/LevelState.hpp"

#include "Controllers/MonsterController.hpp"
#include "res/textures.hpp"

static auto fish_controller_base(FishState& fish, LevelState& level){
  monster_become_active_when_seen(fish, level);
  entity_movement(fish, level);
  entity_kill_player_on_touch(fish, level.player);
  monster_die_when_hit_by_fireball(fish, level);

  for (auto& p : fish.points_generator.items){
    points_particles_controller(p);
  }

  if (fish.was_hit) {
    entity_gravity(fish, level);
    return;
  }

  const auto offset = glm::sin(glfwGetTime()) * fish.max_offset;
  fish.position.y = fish.origin.y + offset;
}

static auto grey_fish_controller(FishState& fish, LevelState& level){
  fish_controller_base(fish, level);
  monster_run_movement_animation(fish, textures::grey_fish_swim);
}

static auto red_fish_controller(FishState& fish, LevelState& level){
  fish_controller_base(fish, level);
  monster_run_movement_animation(fish, textures::red_fish_swim);
}

template<>
struct Controller<FishState>{
  static auto run(FishState& fish, LevelState& level){
    switch(fish.type){
      case FishState::Type::Grey: grey_fish_controller(fish, level); break;
      case FishState::Type::Red: red_fish_controller(fish, level); break;
    }
  }
};
