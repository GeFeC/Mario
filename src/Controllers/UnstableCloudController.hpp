#pragma once

#include "States/BlockState.hpp"
#include "States/LevelState.hpp"
#include "Controllers/CollisionController.hpp"
#include "Window.hpp"

namespace mario{

static auto run_controller(UnstableCloudState& block, LevelState& level){
  auto& player = level.player;

  static constexpr auto FadingSpeedMultiplier = 4.f;
  if (collision_controller::intersects(player, block, -1.f)){
    block.alpha -= window::delta_time * FadingSpeedMultiplier;
    block.alpha = std::max(block.alpha, 0.f);
  }
  else{
    block.alpha += window::delta_time * FadingSpeedMultiplier;
    block.alpha = std::min(block.alpha, 1.f);
  }

  if (block.alpha == 0.f){
    level.hitbox_grid_element(block.position / BlockBase::Size) = LevelState::HitboxState::NonSolid;
  }
  else{
    level.hitbox_grid_element(block.position / BlockBase::Size) = LevelState::HitboxState::Solid;
  }
}

} //namespace mario
