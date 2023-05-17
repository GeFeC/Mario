#pragma once

#include "PolyControllers.hpp"

#include "States/EntityState.hpp"
#include "States/HammerBroState.hpp"
#include "States/LevelState.hpp"
#include "Controllers/EntityController.hpp"
#include "Controllers/PointsParticlesController.hpp"
#include "Util/Util.hpp"
#include "Window.hpp"
#include "config.hpp"
#include "res/textures.hpp"

static auto hammer_controller(HammerState& hammer, LevelState& level){
  hammer.current_texture = &textures::hammer[level.hammer_counter.int_value()];

  if (!hammer.is_active) return;

  entity_kill_player_on_touch(hammer, level.player);
  entity_movement(hammer, level);
  entity_gravity(hammer, level);

  if (hammer.position.y > level.camera_offset.y + config::FrameBufferSize.y) {
    hammer.is_active = false;
    hammer.is_visible = false;
  }
}

template<>
struct Controller<HammerBroState>{
  static auto run(HammerBroState& hammerbro, LevelState& level){
    //Throwing Hammers
    if (level.player.position.y - hammerbro.position.y >= config::FrameBufferSize.y){
      hammerbro.is_active = false;
    }

    for (auto& hammer : hammerbro.hammer_generator.items){
      hammer_controller(hammer, level);
    }

    entity_become_active_when_seen(hammerbro, level);
    if (!hammerbro.is_active) return;

    hammerbro.throw_delay -= window::delta_time;

    auto walk_frames = &textures::hammerbro_walk;

    if (hammerbro.throw_delay <= 0.f && hammerbro.vertical_flip == Drawable::Flip::NoFlip){
      hammerbro.throw_counter.run();

      const auto spawn_hammer_if_can = [&](int value){
        const auto hammers_spawned = hammerbro.hammers_spawned;
        if (hammerbro.throw_counter.int_value() == value && hammers_spawned == value - 1){
          hammerbro.spawn_hammer();
          ++hammerbro.hammers_spawned;
        }
        if (hammerbro.throw_counter.value - value == util::in_range(-0.4f, 0.4f) && hammers_spawned == value){
          walk_frames = &textures::hammerbro_walk_with_hammer;
        }
      };

      spawn_hammer_if_can(1);
      spawn_hammer_if_can(2);
      spawn_hammer_if_can(3);
    }

    for (auto& p : hammerbro.points_generator.items){
      points_particles_controller(p);
    }

    auto copy = textures::hammerbro_walk[0];

    entity_gravity(hammerbro, level);
    entity_run_movement_animation(hammerbro, *walk_frames);

    //Interaction with player
    entity_die_when_hit_by_fireball(hammerbro, level);
    entity_die_when_on_bouncing_block(hammerbro, level);
    entity_kill_player_on_touch(hammerbro, level.player);
    entity_die_when_stomped(hammerbro, level, [&]{
      entity_bounce_out(hammerbro);
    });

    if (hammerbro.vertical_flip == Drawable::Flip::UseFlip) return;

    //Walking
    if (hammerbro.position.x < hammerbro.initial_x - HammerBroState::MaxWalkDistance){
      hammerbro.walk_direction = -hammerbro.walk_direction;
      hammerbro.position.x = hammerbro.initial_x - HammerBroState::MaxWalkDistance;
    }

    if (hammerbro.position.x > hammerbro.initial_x + HammerBroState::MaxWalkDistance){
      hammerbro.walk_direction = -hammerbro.walk_direction;
      hammerbro.position.x = hammerbro.initial_x + HammerBroState::MaxWalkDistance;
    }

    hammerbro.position.x += hammerbro.walk_speed * hammerbro.walk_direction 
      * EntityState::MovementSpeedMultiplier * window::delta_time;


    //Turning
    if (hammerbro.position.x < level.player.position.x){
      hammerbro.direction = EntityState::DirectionRight;
    }
    else{
      hammerbro.direction = EntityState::DirectionLeft;
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
};
