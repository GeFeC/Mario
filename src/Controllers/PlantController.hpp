#pragma once

#include "States/MonsterState.hpp"
#include "States/PlantState.hpp"
#include "States/PlayerState.hpp"
#include "States/LevelState.hpp"
#include "Controllers/EntityController.hpp"
#include "res/textures.hpp"
#include "Window.hpp"

#include <GLFW/glfw3.h>

static auto plant_controller(PlantState& plant, LevelState& level){
  //Interactions with player
  auto& player = level.player;
  entity_kill_player_on_touch(plant, player);

  entity_die_when_hit_by_fireball(plant, player, level.stats);
  const auto is_hit_by_fireball = plant.vertical_flip == Drawable::Flip::UseFlip;
  if (is_hit_by_fireball) plant.is_visible = false;

  const auto offset = window::delta_time * plant.speed;

  //Motion:
  using Direction = PlantState::Direction;
  if (plant.cooldown == 0.f){
    if (plant.direction == Direction::GoingUp && plant.offset < PlantState::MaxOffset){
      plant.offset += offset;
      plant.position.y -= offset;
      return;
    }
    else{
      if (plant.direction == Direction::GoingUp){
        plant.cooldown = plant.cooldown_duration;
      }

      plant.direction = Direction::GoingDown;
    }

    if (plant.direction == Direction::GoingDown && plant.offset > 0.f){
      plant.offset -= offset;
      plant.position.y += offset;
      return;
    }
    else{
      if (plant.direction == Direction::GoingDown){
        plant.cooldown = plant.cooldown_duration;
      }

      plant.direction = Direction::GoingUp;
    }
  }
  plant.cooldown = std::max(plant.cooldown - window::delta_time, 0.f);
}

static auto green_plant_controller(PlantState& plant, LevelState& level){
  plant_controller(plant, level);
  
  //Plant Movement
  entity_run_movement_animation(plant, textures::plant);
}

static auto red_plant_controller(PlantState& plant, LevelState& level){
  plant_controller(plant, level);
  
  //Plant Movement
  entity_run_movement_animation(plant, textures::red_plant);
}
