#pragma once

#include "States/EntityPusherState.hpp"
#include "States/LevelState.hpp"

#include "Controllers/GoombaController.hpp"
#include "Controllers/JumpingKoopaController.hpp"
#include "Controllers/MushroomController.hpp"
#include <type_traits>

static auto pusher_entity_controller(MushroomPusherState& pusher, LevelState& level){
  using MushroomType = MushroomState::Type;
  switch(pusher.entity.type){
    case MushroomType::Red: red_mushroom_controller(pusher.entity, level); break;
    case MushroomType::Green: green_mushroom_controller(pusher.entity, level); break;
  }
}

template<typename Entity>
static auto pusher_entity_controller(EntityPusherState<Entity>& pusher, LevelState& level){
  Controller<Entity>::run(pusher.entity, level);
}

template<typename Entity>
static auto pusher_push_out(EntityPusherState<Entity>& pusher, LevelState& level){
  auto& entity = pusher.entity;

  entity.should_be_pushed_out = true;
  entity.is_visible = true;

  auto& player = level.player;
  const auto block = BouncingBlockState(entity.position / BlockBase::Size);

  if (player.position.x < block.position.x) entity.set_direction(EntityState::DirectionRight);
  if (player.position.x > block.position.x) entity.set_direction(EntityState::DirectionLeft);
}

template<typename Entity>
static auto pusher_controller(
    EntityPusherState<Entity>& pusher, 
    LevelState& level
){
  auto& entity = pusher.entity;

  if (entity.should_be_pushed_out && entity.push_offset < 1.f){
    const auto value = window::delta_time * 2;

    entity.push_offset += value;
    entity.position.y -= value * BlockBase::Size * pusher.push_distance_multiplier;

    if constexpr (std::is_convertible_v<Entity, FlyingKoopaState>){
      entity.initial_position.y = entity.position.y;
    }
  }

  if (entity.push_offset >= 1.f){
    entity.is_active = true;
    pusher_entity_controller(pusher, level);
  }
}
