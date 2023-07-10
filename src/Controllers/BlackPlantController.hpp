#pragma once

#include "Controllers/MonsterController.hpp"
#include "States/BlackPlantState.hpp"
#include "States/LevelState.hpp"

#include "Controllers/PlantController.hpp"
#include "Controllers/EntityController.hpp"
#include "Controllers/FireballController.hpp"

namespace mario{

static auto run_controller(BlackPlantState& plant, LevelState& level){
  monster_controller::become_active_when_seen(plant, level);

  if (!plant.is_active) return;

  for (auto& fireball : plant.fireball_generator.items){
    fireball.explosion.run();

    if (!fireball.is_active) continue;

    entity_controller::gravity(fireball, level);
    entity_controller::kill_player_on_touch(fireball, level.player);
    entity_controller::movement(fireball, level);

    if (fireball.is_on_ground || fireball.position.y > level.max_size().y * BlockBase::Size){
      fireball_controller::reset(fireball);
    }
  }

  if (plant.offset >= PlantState::MaxOffset){
    plant.shot_counter.run();

    if (plant.shot_counter.int_value() == 1 && !plant.was_hit){
      plant.fireball_generator.make_item_if_needed();
      auto& fireball = plant.fireball_generator.item();

      fireball.is_visible = true;
      fireball.is_active = true;
      fireball.gravity = BlackPlantState::FireballGravity;
      fireball.set_direction(plant.fireball_direction, util::random_value(20, 60) / 10.f);
      fireball.position = plant.position + glm::vec2(plant.size.x) / 2.f - fireball.size / 2.f;
      plant.fireball_direction = -plant.fireball_direction;

      plant.shot_counter.reset();
    }
  }
  else{
    plant.shot_counter.reset();
  }

  plant_controller::controller_base(plant, level);
  monster_controller::run_movement_animation(plant, textures::black_plant);
}

} //namespace mario
