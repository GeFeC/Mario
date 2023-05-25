#pragma once

#include "PolyControllers.hpp"

#include "Controllers/PointsParticlesController.hpp"
#include "States/MonsterState.hpp"
#include "States/PlantState.hpp"
#include "States/PlayerState.hpp"
#include "States/LevelState.hpp"
#include "Controllers/EntityController.hpp"
#include "res/textures.hpp"
#include "Window.hpp"

#include <GLFW/glfw3.h>

static auto plant_controller_base(PlantState& plant, LevelState& level){
  //Points
  for (auto& p : plant.points_generator.items){
    points_particles_controller(p);
  }

  //Interactions with player
  auto& player = level.player;

  entity_kill_player_on_touch(plant, player);
  entity_die_when_hit_by_fireball(plant, level);

  if (plant.was_hit) plant.is_visible = false;

  const auto offset = window::delta_time * plant.speed;

  //Motion:
  using Direction = PlantState::Direction;
  if (plant.current_cooldown == 0.f){
    if (plant.direction == Direction::GoingUp && plant.offset < PlantState::MaxOffset){
      plant.offset += offset;
      plant.position.y -= offset * plant.vertical_flip;
      return;
    }
    else{
      if (plant.direction == Direction::GoingUp){
        plant.current_cooldown = plant.cooldown_duration;
      }

      plant.direction = Direction::GoingDown;
    }

    if (plant.direction == Direction::GoingDown && plant.offset > 0.f){
      plant.offset -= offset;
      plant.position.y += offset * plant.vertical_flip;
      return;
    }
    else{
      if (plant.direction == Direction::GoingDown){
        plant.current_cooldown = plant.cooldown_duration;
      }

      plant.direction = Direction::GoingUp;
    }
  }
  plant.current_cooldown = std::max(plant.current_cooldown - window::delta_time, 0.f);
}

static auto green_plant_controller(PlantState& plant, LevelState& level){
  plant_controller_base(plant, level);
  
  entity_run_movement_animation(plant, textures::plant);
}

static auto red_plant_controller(PlantState& plant, LevelState& level){
  plant_controller_base(plant, level);
  
  entity_run_movement_animation(plant, textures::red_plant);
}

template<>
struct Controller<PlantState>{
  static auto run(PlantState& plant, LevelState& level){
    using Type = PlantState::Type;
    switch(plant.type){
      case Type::Green: green_plant_controller(plant, level); return;
      case Type::Red: red_plant_controller(plant, level); return;
    }
  }
};
