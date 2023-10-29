#pragma once

#include "Controllers/EntityController.hpp"
#include "Controllers/MonsterController.hpp"
#include "States/BlockState.hpp"
#include "States/LakitoState.hpp"
#include "States/LevelState.hpp"
#include "Window.hpp"
#include "res/textures.hpp"

namespace mario::lakito_controller{

static auto run_thrown_spike_controller(LakitoState::ThrownSpikeState& thrown_spike, LevelState& level){
  entity_controller::kill_player_on_touch(thrown_spike, level);
  entity_controller::handle_gravity(thrown_spike, level);
  monster_controller::run_movement_animation(thrown_spike, textures::lakito_throw);

  if (thrown_spike.is_on_ground){
    auto& new_spike = level.game_objects.push(SpikeState::make({ 0, 0 }));
    new_spike.position = thrown_spike.position;
    new_spike.is_active = true;

    if (new_spike.position.x < level.player.position.x){
      new_spike.set_direction(util::Direction::right());
    }

    thrown_spike.is_active = false;
    thrown_spike.position.y = util::BigValue;
    thrown_spike.is_on_ground = false;
  }
}

} //namespace mario::lakito_controller

namespace mario{

static auto run_controller(LakitoState& lakito, LevelState& level){
  monster_controller::become_active_when_seen(lakito, level);
  monster_controller::die_when_stomped(lakito, level, [&]{
    monster_controller::bounce_die(lakito, level.stats);
  });
  monster_controller::handle_points_particles(lakito);
  monster_controller::die_when_hit_by_fireball(lakito, level);

  entity_controller::handle_movement(lakito, level);
  entity_controller::kill_player_on_touch(lakito, level);

  if (lakito.vertical_flip.is_flipped()){
    entity_controller::handle_gravity(lakito, level);
  }

  lakito_controller::run_thrown_spike_controller(lakito.thrown_spike, level);

  if (!lakito.is_active || lakito.vertical_flip.is_flipped()) return;

  //Movement

  if (level.type == LevelState::Type::Vertical){
    //set acceleration to 0 when outside the world borders:
    const auto lakito_pos_min = 2 * -lakito.size.x;
    const auto lakito_pos_max = LevelState::MaxVerticalLevelSize.x * BlockBase::Size + lakito.size.x;

    if (lakito.position.x != util::in_range(lakito_pos_min, lakito_pos_max)){
      lakito.acceleration.left = lakito.acceleration.right = 0.f;
    }
  }

  auto speed_boost_multiplier = std::max(level.player.total_speed(), 3.f);

  const auto half_of_player_view = BlockBase::Size * LevelState::BlocksInRow / 2.f;
  if (std::abs(level.player.position.x - lakito.position.x) > half_of_player_view){
    speed_boost_multiplier = 10.f;
  }

  static constexpr auto MaxAccelerationLeft = 3.f;
  static constexpr auto MaxAccelerationRight = 10.f;

  const auto speed_boost = window::delta_time * speed_boost_multiplier;

  if (lakito.position.x > level.player.position.x && lakito.acceleration.left < MaxAccelerationLeft){
    lakito.acceleration.left += speed_boost;
    lakito.acceleration.right -= speed_boost;
  }

  if (lakito.position.x < level.player.position.x && lakito.acceleration.left < MaxAccelerationRight){
    lakito.acceleration.left -= speed_boost;
    lakito.acceleration.right += speed_boost;
  }

  //Throwing spike
  lakito.throw_counter.run(window::delta_time);

  if (lakito.throw_counter.value == util::in_range(9.f, 10.f)){
    lakito.current_texture = &textures::lakito_hidden;

    static constexpr auto ThrownSpikeBouncePower = -20.f;
    if (!lakito.was_spike_thrown){
      auto& spike = lakito.thrown_spike;
      spike.position = lakito.position;
      spike.is_active = true;
      spike.gravity = ThrownSpikeBouncePower;
      spike.is_on_ground = false;
      lakito.was_spike_thrown = true;
    }
  }
  else{
    lakito.current_texture = &textures::lakito_shown;
      lakito.was_spike_thrown = false;
  }
}

} //namespace mario
