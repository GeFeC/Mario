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

  //Existing fireballs movement and reseting
  for (auto& fireball : plant.fireball_generator.items){
    fireball.explosion.run();

    if (!fireball.is_active) continue;

    entity_controller::handle_gravity(fireball, level);
    entity_controller::handle_movement(fireball, level);
    entity_controller::kill_player_on_touch(fireball, level);

    const auto fireball_out_of_screen = fireball.position.y > level.max_size().y * BlockBase::Size;
    const auto fireball_stopped = fireball.acceleration.left == 0.f && fireball.acceleration.right == 0.f;
    if (fireball.is_on_ground || fireball_out_of_screen || fireball_stopped){
      fireball_controller::reset(fireball);
    }
  }

  //Shooting
  if (plant.offset >= PlantState::MaxOffset){
    plant.shot_counter.run(window::delta_time);

    if (
			entity_controller::is_player_nearby(plant, level.player) && 
			plant.shot_counter.int_value() == 1 && 
			!plant.was_hit
		){
			sounds::sounds[sounds::Shot].play();

      plant.fireball_generator.make_item_if_needed();
      auto& fireball = plant.fireball_generator.item();

      const auto is_flipped = plant.vertical_flip.is_flipped();
      const auto fireball_position_y = is_flipped 
        * plant.size.y 
        + plant.vertical_flip.as_int() 
        * BlockBase::Size 
        / 2.f;

      const auto fireball_position = plant.position
        + glm::vec2(plant.size.x / 2.f, fireball_position_y)
        - fireball.size / 2.f;

      fireball.shoot(
        fireball_position, 
        plant.fireball_direction, 
        util::random_value(20, 60) / 10.f * plant.shot_boost
      );

      fireball.gravity = BlackPlantState::FireballGravity * !is_flipped;
      plant.fireball_direction = -plant.fireball_direction;
      plant.shot_counter.reset();
    }
  }
  else{
    plant.shot_counter.reset();
  }

  plant_controller::run_controller_base(plant, level);
  entity_controller::run_movement_animation(plant, textures::black_plant);
}

} //namespace mario
