#pragma once

#include "States/FireBarState.hpp"
#include "States/LevelState.hpp"
#include "Controllers/EntityController.hpp"

static auto fire_bar_controller(FireBarState& bar, LevelState& level){
  for (int i = 0; i < bar.fireballs.size(); ++i){
    auto& fireball = bar.fireballs[i];
    fireball.current_texture = &textures::fireball[level.fireball_counter.int_value()];

    const auto y = BlockBase::Size * i * 0.8f;
    const auto rotation = glm::vec2(
      -y * glm::sin(LevelState::timer * FireBarState::RotationSpeed),
      y * glm::cos(LevelState::timer * FireBarState::RotationSpeed)
    );
    fireball.position = bar.position + rotation;

    entity_kill_player_on_touch(fireball, level.player);
  }

}
