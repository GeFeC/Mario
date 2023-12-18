#pragma once

#include "States/EntityState.hpp"
#include "States/HammerBroState.hpp"
#include "States/LevelState.hpp"

#include "Controllers/MonsterController.hpp"
#include "Controllers/PointsParticlesController.hpp"

#include "Window.hpp"
#include "config.hpp"
#include "res/textures.hpp"

namespace mario::hammerbro_controller{

static auto run_hammer_controller(HammerState& hammer, LevelState& level){
  hammer.current_texture = &textures::hammer[level.hammer_counter.int_value()];

  if (!hammer.is_active) return;

  entity_controller::kill_player_on_touch(hammer, level);
  entity_controller::handle_movement(hammer, level);
  entity_controller::handle_gravity(hammer, level);

  if (hammer.position.y > level.camera_offset.y + config::FrameBufferSize.y) {
    hammer.is_active = false;
    hammer.is_visible = false;
  }
}

static auto run_controller_base(
    HammerBroState& hammerbro, 
    LevelState& level,
    const std::array<renderer::Texture, 2>& walk_frames,
    const std::array<renderer::Texture, 2>& walk_frames_with_hammer
){
  //Throwing Hammers
  if (level.player.position.y - hammerbro.position.y >= config::FrameBufferSize.y){
    hammerbro.is_active = false;
  }

  for (auto& hammer : hammerbro.hammer_generator.items){
    run_hammer_controller(hammer, level);
  }

  monster_controller::become_active_when_seen(hammerbro, level);
  if (!hammerbro.is_active) return;

  hammerbro.throw_delay -= window::delta_time;

  auto current_walk_frames = &walk_frames;

  if (hammerbro.throw_delay <= 0.f && !hammerbro.vertical_flip.is_flipped()){
    hammerbro.throw_counter.run(window::delta_time);

    const auto spawn_hammer_if_can = [&](int hammer_index){
      const auto hammers_spawned = hammerbro.hammers_spawned;
      if (hammerbro.throw_counter.int_value() == hammer_index && hammers_spawned == hammer_index - 1){
        hammerbro.spawn_hammer();
        ++hammerbro.hammers_spawned;
      }

      const auto spawn_cooldown_ended = hammerbro.throw_counter.value - hammer_index == util::in_range(-0.4f, 0.4f);

      if (spawn_cooldown_ended && hammers_spawned == hammer_index){
        current_walk_frames = &walk_frames_with_hammer;
      }
    };

    for (int i = 0; i < hammerbro.hammers_count; ++i) spawn_hammer_if_can(i + 1);
  }

  for (auto& p : hammerbro.points_generator.items){
    points_particles_controller::run(p);
  }

  auto copy = textures::hammerbro_walk[0];

  entity_controller::handle_gravity(hammerbro, level);
  monster_controller::run_movement_animation(hammerbro, *current_walk_frames);

  //Interaction with player
  monster_controller::die_when_hit_by_fireball(hammerbro, level);
  monster_controller::die_when_on_bouncing_block(hammerbro, level);

  if (hammerbro.vertical_flip.is_flipped()) return;

  monster_controller::kill_player_on_touch(hammerbro, level);
  monster_controller::die_when_stomped(hammerbro, level, [&]{
    monster_controller::bounce_out(hammerbro);
  });

  //Walking
  if (hammerbro.position.x < hammerbro.initial_x - HammerBroState::MaxWalkDistance){
    hammerbro.walk_direction = -hammerbro.walk_direction;
    hammerbro.position.x = hammerbro.initial_x - HammerBroState::MaxWalkDistance;
  }

  if (hammerbro.position.x > hammerbro.initial_x + HammerBroState::MaxWalkDistance){
    hammerbro.walk_direction = -hammerbro.walk_direction;
    hammerbro.position.x = hammerbro.initial_x + HammerBroState::MaxWalkDistance;
  }

  hammerbro.position.x += hammerbro.walk_speed 
    * hammerbro.walk_direction.as_int()
    * EntityState::MovementSpeedMultiplier 
    * window::delta_time;

  //Turning
  if (hammerbro.position.x < level.player.position.x){
    hammerbro.direction = util::Direction::right();
  }
  else{
    hammerbro.direction = util::Direction::left();
  }

  //Jumping
  using JumpState = HammerBroState::JumpState;
  if (hammerbro.started_jumping && hammerbro.is_on_ground){
    if (hammerbro.jump_state == JumpState::Up) hammerbro.jump_state = JumpState::Down;
    else hammerbro.jump_state = JumpState::Up;
    hammerbro.started_jumping = false;
  }

  if (hammerbro.jump_state == JumpState::Up && hammerbro.gravity >= 0.f){
    hammerbro.should_collide = true;
  }

  if (hammerbro.jump_state == JumpState::Down && hammerbro.gravity >= 30.f){
    hammerbro.should_collide = true;
  }

  hammerbro.jump_delay -= window::delta_time;
  if (hammerbro.jump_delay > 0.f) return;

  hammerbro.jump_delay = HammerBroState::new_jump_delay();

  if (hammerbro.jump_state == JumpState::Up) hammerbro.gravity = HammerBroState::JumpPower;
  else hammerbro.gravity = HammerBroState::WeakJumpPower;
 
  hammerbro.should_collide = false;
  hammerbro.started_jumping = true;
  hammerbro.is_on_ground = false;

  hammerbro.throw_counter.reset();
  hammerbro.hammers_spawned = 0;
  hammerbro.throw_delay = HammerBroState::new_throw_delay();
}

static auto run_green_hammerbro_controller(HammerBroState& bro, LevelState& level){
  run_controller_base(
      bro, 
      level, 
      textures::hammerbro_walk, 
      textures::hammerbro_walk_with_hammer
  );
}

static auto run_red_hammerbro_controller(HammerBroState& bro, LevelState& level){
  run_controller_base(
      bro, 
      level, 
      textures::red_hammerbro_walk, 
      textures::red_hammerbro_walk_with_hammer
  );
}

} //namespace mario::hammerbro_controller

namespace mario{

static auto run_controller(HammerBroState& bro, LevelState& level){
  using Type = HammerBroState::Type;
  switch(bro.type){
    case Type::Green: hammerbro_controller::run_green_hammerbro_controller(bro, level); break;
    case Type::Red: hammerbro_controller::run_red_hammerbro_controller(bro, level); break;
  }
}

} //namespace mario
