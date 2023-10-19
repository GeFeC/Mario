#pragma once

#include "States/BlockState.hpp"
#include "States/LevelState.hpp"
#include "Controllers/CollisionController.hpp"
#include "Window.hpp"

namespace mario{

static auto run_controller(UnstableCloudState& block, LevelState& level){
  auto& player = level.player;

  static constexpr auto FadingSpeedMultiplier = 4.f;

  using collision_controller::CollisionPadding;

  /*
   * Player detection area size is increased depending on the gravity flip
   * so unstable clouds can completely fade out above player's head
  */
  const auto player_detection_area_size = player.size + glm::vec2(0.f, CollisionPadding / 2);

  const auto player_detection_area = player.gravity_flip.is_flipped()
    ? collision_controller::Rect{ player.position, player_detection_area_size }
    : collision_controller::Rect{ player.position - glm::vec2(0.f, CollisionPadding / 2), player_detection_area_size };

  if (collision_controller::intersects(player_detection_area, block, -1.f)){
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
